#include "HttpController.hpp"
// #include <fstream>

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
        request.SetHeader(headerName, Tools::ToLower(Tools::Trim(headerValue, WhiteSpaces)));
}
void HttpController::ParseRequestHeaders(Request& request)
{
    std::istringstream iss(request.GetRequestContent());
    std::string line;

    while (std::getline(iss, line))
    {    
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
            line.pop_back();

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
void HttpController::ParseBody(Request& request)
{
    int contentLength = std::atoi(request.GetHeader("content-length").c_str());
    std::string tmpRequestContent = request.GetRequestContent();
    int currentLength = tmpRequestContent.length();

    if(currentLength > contentLength)
        request.SetBody(tmpRequestContent.substr(0, contentLength));
    else if (currentLength < contentLength)
    {
        tmpRequestContent += Tools::Recv(request.GetSocketId(), contentLength - currentLength);
        request.SetBody(tmpRequestContent);
    }
    else
        request.SetBody(tmpRequestContent);

    request.Status(Completed);
}
void HttpController::ParseChunked(Request& request)
{
    while(request.GetRequestContent().find("0\r\n\r\n") == std::string::npos)
       request.AppendRequestContent(Tools::Recv(request.GetSocketId(), RECV_SIZE));

    std::string requestContent = request.GetRequestContent();
    std::string block = requestContent.substr(0, requestContent.find("\r\n"));

    int blockSize = static_cast<int>(std::strtol(block.c_str(), NULL, 16));
    if(blockSize == 0)
    {
        request.Status(Completed);
        return;
    }

    // Why am I adding 2 and 4 because when I use .find("0\r\n\r\n") 
    // it removes the line that doesn't have \r\n in it after 
    // I need to skip \r\n the chunk, that's why I plus 2 and 4
    request.SetChunk(requestContent.substr(block.length() + 2, blockSize));
    std::string newRequestContent = requestContent.substr(blockSize + block.length() + 4);
    request.SetRequestContent(newRequestContent);
    request.Status(InProgress);
}
bool HttpController::ProcessHTTPRequest(int socketId)
{
    while(!_requests[socketId].ReadFurther())
    {   
        _requests[socketId].AppendRequestContent(Tools::Recv(socketId, RECV_SIZE));
        if(_requests[socketId].GetRequestContent().find("\r\n\r\n") != std::string::npos)
            _requests[socketId].ReadFurther(true);
    }
    if(!_requests[socketId].AreHeadersFinished())
        ParseRequestHeaders(_requests[socketId]);

    if(_requests[socketId].HasHeader("content-length"))
        ParseBody(_requests[socketId]);
    else if(_requests[socketId].HasHeader("transfer-encoding") &&
        !_requests[socketId].GetHeader("transfer-encoding").compare("chunked"))
        ParseChunked(_requests[socketId]);
    else
        _requests[socketId].Status(Completed);

    return _requests[socketId].Status();
}
bool HttpController::HttpRequest(int readSocket)
{   
    if(!CheckRequestIn(readSocket))
        CreateNewRequest(readSocket);
    return ProcessHTTPRequest(readSocket);
}

void HttpController::HttpResponse(int readSocket) //* Needs some work *
{
    const char *http_response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
    send(readSocket, http_response, strlen(http_response), 0);







    
    _requests.erase(readSocket);
}