#include "HttpRequestController.hpp"

bool HttpRequestController::CheckRequestIn(int socketId) { return _requests.count(socketId) > 0 ? true : false; }
void HttpRequestController::CreateNewRequest(int socketId) { _requests[socketId] = Request(); }
void HttpRequestController::FirstRequestLine(const std::string& line, Request& request)
{
    std::istringstream iss(line);
    std::string method, path, version;
    iss >> method >> path >> version;
    HttpMethod parsedMethod = NONE;
    if(method == "GET")
        parsedMethod = GET;
    else if(method == "POST")
        parsedMethod = POST;
    else if(method == "DELETE")
        parsedMethod = DELETE;
    else if(method == "HEAD")
        parsedMethod = HEAD;

    request.SetMethod(parsedMethod);
    request.SetPath(path);
    request.SetVersion(version);
}
void HttpRequestController::ParseHeaderLine(const std::string& line, Request& request)
{
    std::istringstream iss(line);
    std::string headerName, headerValue;

    std::getline(iss, headerName, ':');
    std::getline(iss, headerValue);

    if(!request.HasHeader(headerName))
        request.SetHeader(Tools::ToLower(headerName), Tools::Trim(headerValue, WhiteSpaces));
}
RequestResult HttpRequestController::ParseChunked(Request& request)
{
    std::string requestContent = request.GetRequestContent();
    if(request.GetChunkSize() == -1)
    {
        std::string block = requestContent.substr(0, requestContent.find("\r\n"));
        request.SetChunkSize(static_cast<int>(std::strtol(block.c_str(), NULL, 16)));
    }

    std::string chunkSizeStr = Tools::ToString(request.GetChunkSize());
    request.SetChunk(requestContent.substr(chunkSizeStr.length() + 2, request.GetChunkSize()));
    std::string newRequestContent = requestContent.substr(request.GetChunkSize() + chunkSizeStr.length() + 4);

    chunkSizeStr = newRequestContent.substr(0, newRequestContent.find("\r\n"));
    request.SetChunkSize(static_cast<int>(std::strtol(chunkSizeStr.c_str(), NULL, 16)));
    if(request.GetChunkSize() == 0)
        return Success;
    
    request.SetRequestContent(newRequestContent);
    return Chunked;
}

std::string HttpRequestController::ExtractBoundary(const std::string& contentType)
{
    std::string boundaryKey = "boundary=";
    size_t boundaryPos = contentType.find(boundaryKey);
    if(boundaryPos != std::string::npos)
    {
        std::string key = contentType.substr(boundaryPos + boundaryKey.length());
        if(key != "")
            return "--" + key + "--";
    }
    return "";
}

RequestResult HttpRequestController::ParseBody(Request& request)
{
    int contentLength = atoi(request.GetHeader("content-length").c_str());

    if(request.GetBoundaryKey() != "")
    {
        size_t boundaryPos = request.GetRequestContent().rfind(request.GetBoundaryKey());
        request.SetBody(request.GetRequestContent().substr(0, boundaryPos));
        if (boundaryPos == std::string::npos)
            return Multipart;
    }
    else
        request.SetBody(request.GetRequestContent().substr(0, contentLength));

    return Success;
}

void HttpRequestController::ParseRequestHeaders(Request& request)
{
    size_t EndPos = request.GetRequestContent().find("\r\n\r\n");
    if (EndPos > LimitHeaders)
        return;
    
    std::string content = request.GetRequestContent();
    std::istringstream iss(content);
    std::string line;
    size_t headersEndPos = std::string::npos;

    while(std::getline(iss, line))
    {   
        while(!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        if(line.empty())
        {
            headersEndPos = iss.tellg();
            break;
        }
        
        if(line == request.GetBoundaryKey())
            continue;

        if(!request.FirstLineCheck())
        {
            FirstRequestLine(line, request);
            request.FirstLineCheck(true);
        }
        else if(line.find(":") != std::string::npos)
            ParseHeaderLine(line, request);
    }

    if(request.GetHeader("content-type").find("multipart/form-data") != std::string::npos)
    {
        std::string boundary = ExtractBoundary(request.GetHeader("content-type"));
        request.SetBoundaryKey(boundary);
    }


    if (headersEndPos != std::string::npos)
    {
        std::string newRequestContent = "";
        if(headersEndPos  >= content.size())
            request.SetRequestContent(newRequestContent);
        else
        {
            newRequestContent = content.substr(headersEndPos );
            request.SetRequestContent(newRequestContent);
        }
    }
}
void HttpRequestController::CheckMultiPart(Request& request, int socketId)
{
    std::string boundary = request.GetBoundaryKey();

    if(boundary.empty())
        return;
    Logger::LogMsg(DEBUG, "Uploading..", socketId);
    ParseRequestHeaders(request);
}

RequestResult HttpRequestController::ProcessHTTPRequest(int socketId)
{
    RequestResult requestResult = Success;
    while(true)
    {
        char requestBuffer[MessageBuffer];
        ssize_t bytesRead = -1;

        requestResult = Tools::Recv(socketId, requestBuffer, bytesRead);
        if(requestResult != Success)
            return requestResult;

        if(bytesRead == -1)
            break;

        _requests[socketId].AppendRequestContent(requestBuffer, bytesRead);
    }

    ParseRequestHeaders(_requests[socketId]);
    CheckMultiPart(_requests[socketId], socketId);

    if(_requests[socketId].HasHeader("content-length"))
        requestResult = ParseBody(_requests[socketId]);
    else if(_requests[socketId].HasHeader("transfer-encoding") &&
        !_requests[socketId].GetHeader("transfer-encoding").compare("chunked"))
        requestResult = ParseChunked(_requests[socketId]);

    _requests[socketId].ClearContent();
    return requestResult;
}
RequestResult HttpRequestController::HttpRequest(int readSocket)
{
    if(!CheckRequestIn(readSocket))
        CreateNewRequest(readSocket);

    RequestResult requestResult = ProcessHTTPRequest(readSocket);
    if(requestResult == ClosedConnection)
        _requests.erase(readSocket);

    return requestResult;
}
Request& HttpRequestController::GetRequest(int index) { return _requests[index]; }
void HttpRequestController::ClearRequest(int index) { _requests.erase(index); }