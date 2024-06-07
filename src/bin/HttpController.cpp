#include "HttpController.hpp"

bool HttpController::CheckRequestIn(int socketId) { return _requests.count(socketId) > 0 ? true : false; }
void HttpController::CreateNewRequest(int socketId) { _requests[socketId] = Request(); }
void HttpController::FirstRequestLine(const std::string& line, Request& request)
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
    else if(method == "PUT")
        parsedMethod = PUT;
    else if(method == "HEAD")
        parsedMethod = HEAD;

    request.SetMethod(parsedMethod);
    request.SetPath(path);
    request.SetVersion(version);
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
    std::string headers = request.GetRequestContent().substr(0, request.GetRequestContent().find("\r\n\r\n"));
    std::string newRequestContent = request.GetRequestContent().substr(request.GetRequestContent().find("\r\n\r\n") + 4);
    if(headers.length() > LimitHeaders || newRequestContent.length() > LimitRequest)
        return request.IsOverflow(true);
    std::istringstream iss(headers);
    std::string line;
    bool firstLineCheck = false;

    while(std::getline(iss, line))
    {    
        while(!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        if(!firstLineCheck)
        {
            FirstRequestLine(line, request);
            firstLineCheck = true;
        }
        else
            ParseHeaderLine(line, request);
    }
    request.SetRequestContent(newRequestContent);
    request.ReadAllDataFromSocket(true);
}
void HttpController::ParseBody(Request& request)
{
    int contentLength = std::atoi(request.GetHeader("content-length").c_str());
    request.SetBody(request.GetRequestContent().substr(0, contentLength));
}
RequestResult HttpController::ParseChunked(Request& request)
{
    std::string requestContent = request.GetRequestContent();
    if(request.GetChunkSize() == -1)
    {
        std::string block = requestContent.substr(0, requestContent.find("\r\n"));
        request.SetChunkSize(static_cast<int>(std::strtol(block.c_str(), NULL, 16)));
    }

    // Why am I adding 2 and 4 because when I use .find("0\r\n\r\n") 
    // it removes the line that doesn't have \r\n in it after 
    // I need to skip \r\n the chunk, that's why I plus 2 and 4
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
RequestResult HttpController::ProcessHTTPRequest(int socketId)
{
    RequestResult requestResult = Success;
    if(!_requests[socketId].ReadAllDataFromSocket())
    {
        while(true)
        {
            char requestBuffer[MessageBuffer];
            requestResult = Tools::Recv(socketId, requestBuffer);
            if(requestResult != Success)
                return requestResult;
            if(std::string(requestBuffer).length() == 0)
                break;
            _requests[socketId].AppendRequestContent(std::string(requestBuffer));
        }
        ParseRequestHeaders(_requests[socketId]);
    }

    if(_requests[socketId].IsOverflow())
    {
        _requests[socketId].ClearRequestContent();
        return requestResult;
    }

    if(_requests[socketId].HasHeader("content-length"))
        ParseBody(_requests[socketId]);
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
    if(_requests[readSocket].IsOverflow())
    {
        std::cout<<"413"<<std::endl;
        const char *http_response = "HTTP/1.1 413 Content Too Large\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<title>Welcome to nginx!</title>\n<style>\nhtml { color-scheme: light dark; }\nbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n</style>\n</head>\n<body>\n<h1>Welcome to nginx!</h1>\n<p>If you see this page, the nginx web server is successfully installed and working. Further configuration is required.</p>\n<p>For online documentation and support please refer to <a href=\"http://nginx.org/\">nginx.org</a>.<br/>Commercial support is available at <a href=\"http://nginx.com/\">nginx.com</a>.</p>\n<p><em>Thank you for using nginx.</em></p>\n</body>\n</html>";
        std::cout<<"Send: "<<send(readSocket, http_response, strlen(http_response), 0)<<std::endl;
    }
    else
    {   
        std::cout<<"200"<<std::endl;
        const char *http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<title>Welcome to nginx!</title>\n<style>\nhtml { color-scheme: light dark; }\nbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n</style>\n</head>\n<body>\n<h1>Welcome to nginx!</h1>\n<p>If you see this page, the nginx web server is successfully installed and working. Further configuration is required.</p>\n<p>For online documentation and support please refer to <a href=\"http://nginx.org/\">nginx.org</a>.<br/>Commercial support is available at <a href=\"http://nginx.com/\">nginx.com</a>.</p>\n<p><em>Thank you for using nginx.</em></p>\n</body>\n</html>";
        std::cout<<"Send: "<<send(readSocket, http_response, strlen(http_response), 0)<<std::endl;
    }

    std::cout<<_requests[readSocket].GetRequestContent()<<std::endl;
    _requests.erase(readSocket);
}