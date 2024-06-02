#include "HttpController.hpp"

bool HttpController::CheckRequestIn(int socketId) { return _requests.count(socketId) > 0 ? true : false; }
void HttpController::CreateNewRequest(int socketId) { _requests[socketId] = Request(); _requests[socketId].SetSocketId(socketId); }
void HttpController::FirstRequestLine(const std::string& line, Request& request)
{
    std::istringstream iss(line);
    std::string method, path, version;
    iss >> method >> path >> version;
    HttpMethod parsedMethod = NONE;
    if (method == "GET")
        parsedMethod = GET;
    else if (method == "POST")
        parsedMethod = POST;
    else if (method == "DELETE")
        parsedMethod = DELETE;
    else if (method == "PUT")
        parsedMethod = PUT;
    else if (method == "HEAD")
        parsedMethod = HEAD;

    request.SetMethod(parsedMethod);
    request.SetPath(path);
    request.SetVersion(version);

    request.SetFirstLineCheck(true);
}
void HttpController::ParseHeaderLine(const std::string& line, Request& request)
{
    std::istringstream iss(line);
    std::string headerName, headerValue;

    std::getline(iss, headerName, ':');
    std::getline(iss, headerValue);

    if(!request.HasHeader(headerName))
        request.SetHeader(Tools::ToLower(headerName), Tools::ToLower(Tools::Trim(headerValue, WhiteSpaces)));
}
void HttpController::ParseRequestHeaders(Request& request)
{
    std::istringstream iss(request.GetRequestContent());
    std::string line;

    while (std::getline(iss, line))
    {    
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        if (line.empty())
            break;

        if (!request.GetFirstLineCheck())
            FirstRequestLine(line, request);
        else
            ParseHeaderLine(line, request);
    }
    std::string newRequestContent = request.GetRequestContent().substr(request.GetRequestContent().find("\r\n\r\n") + 4);
    request.SetRequestContent(newRequestContent);
    request.AreHeadersFinished(true);
}
RequestResult HttpController::ParseBody(Request& request)
{
    int contentLength = std::atoi(request.GetHeader("content-length").c_str());
    std::string tmpRequestContent = request.GetRequestContent();
    int currentLength = tmpRequestContent.length();

    if(currentLength > contentLength)
        request.SetBody(tmpRequestContent.substr(0, contentLength));
    else if (currentLength < contentLength)
    {
        char requestBuffer[LimitRequestBody];
        RequestResult recvResult = Tools::Recv(request.GetSocketId(), requestBuffer, LimitRequestBody);
        if(recvResult != Success)
            return recvResult;
        request.SetBody(tmpRequestContent + std::string(requestBuffer));
    }
    else
        request.SetBody(tmpRequestContent);

    return Success;
}
RequestResult HttpController::ParseChunked(Request& request)
{
    if(request.GetRequestContent().find("0\r\n\r\n") == std::string::npos)
    {
        char requestBuffer[LimitChunk];
        RequestResult recvResult = Tools::Recv(request.GetSocketId(), requestBuffer, LimitChunk);
        if(recvResult != Success)
            return recvResult;
        request.AppendRequestContent(std::string(requestBuffer));
    }

    std::string requestContent = request.GetRequestContent();
    std::string block = requestContent.substr(0, requestContent.find("\r\n"));

    if(request.GetChunkSize() == -1)
        request.SetChunkSize(static_cast<int>(std::strtol(block.c_str(), NULL, 16)));

    // Why am I adding 2 and 4 because when I use .find("0\r\n\r\n") 
    // it removes the line that doesn't have \r\n in it after 
    // I need to skip \r\n the chunk, that's why I plus 2 and 4
    request.SetChunk(requestContent.substr(block.length() + 2, request.GetChunkSize()));
    std::string newRequestContent = requestContent.substr(request.GetChunkSize() + block.length() + 4);

    block = newRequestContent.substr(0, newRequestContent.find("\r\n"));
    int blockSize = static_cast<int>(std::strtol(block.c_str(), NULL, 16));
    if(blockSize == 0)
        return Success;

    request.SetChunkSize(blockSize);
    request.SetRequestContent(newRequestContent);
    return Chunked;
}
RequestResult HttpController::ProcessHTTPRequest(int socketId)
{
    RequestResult requestResult = Success;
    while(!_requests[socketId].ReadFurther())
    {
        char requestBuffer[RecvSize];
        requestResult = Tools::Recv(socketId, requestBuffer, RecvSize);
        if(requestResult != Success)
            return requestResult;

        _requests[socketId].AppendRequestContent(std::string(requestBuffer));
        if(_requests[socketId].GetRequestContent().find("\r\n\r\n") != std::string::npos)
            _requests[socketId].ReadFurther(true);
    }

    if(!_requests[socketId].AreHeadersFinished())
        ParseRequestHeaders(_requests[socketId]);

    if(_requests[socketId].HasHeader("content-length"))
        requestResult = ParseBody(_requests[socketId]);
    else if(_requests[socketId].HasHeader("transfer-encoding") &&
        !_requests[socketId].GetHeader("transfer-encoding").compare("chunked"))
        requestResult = ParseChunked(_requests[socketId]);
    
    return requestResult;
}
RequestResult HttpController::HttpRequest(int readSocket)
{   
    if(!CheckRequestIn(readSocket))
        CreateNewRequest(readSocket);

    RequestResult requestResult = ProcessHTTPRequest(readSocket);
    if(requestResult != Success && requestResult != Chunked)
        _requests.erase(readSocket);

    return requestResult;
}




void HttpController::HttpResponse(int readSocket) //* Needs some work *
{
    const char *http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<title>Welcome to nginx!</title>\n<style>\nhtml { color-scheme: light dark; }\nbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n</style>\n</head>\n<body>\n<h1>Welcome to nginx!</h1>\n<p>If you see this page, the nginx web server is successfully installed and working. Further configuration is required.</p>\n<p>For online documentation and support please refer to <a href=\"http://nginx.org/\">nginx.org</a>.<br/>Commercial support is available at <a href=\"http://nginx.com/\">nginx.com</a>.</p>\n<p><em>Thank you for using nginx.</em></p>\n</body>\n</html>";
    std::cout<<"Send: "<<send(readSocket, http_response, strlen(http_response), 0)<<std::endl;

    _requests.erase(readSocket);
}