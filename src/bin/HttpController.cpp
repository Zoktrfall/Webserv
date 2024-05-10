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
void HttpController::ParseRequestHeaders(std::string& requestContent, Request& request)
{
    std::istringstream iss(requestContent);
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
}
void HttpController::ParseBody(std::string& requestContent, Request& request)
{
    int contentLength = std::atoi(request.GetHeader("content-length").c_str());
    requestContent = requestContent.substr(requestContent.find("\r\n\r\n") + 4);
    int currentLength = requestContent.length();

    if(currentLength > contentLength)
        request.SetBody(requestContent.substr(0, contentLength));
    else if (currentLength < contentLength)
    {
        requestContent += Tools::Recv(request.GetSocketId(), contentLength - currentLength);
        currentLength = requestContent.length();
        request.SetBody(requestContent);
    }
    else
        request.SetBody(requestContent);
}


bool HttpController::ProcessHTTPRequest(int socketId)
{
    std::string requestContent = "";
    while(!_requests[socketId].ReadFurther())
    {   
        requestContent += Tools::Recv(socketId, RECV_SIZE);
        if(requestContent.find("\r\n\r\n") != std::string::npos)
            _requests[socketId].ReadFurther(true);
    }

    // std::cout<<requestContent<<std::endl;
    ParseRequestHeaders(requestContent, _requests[socketId]);

    if(_requests[socketId].HasHeader("content-length"))
        ParseBody(requestContent, _requests[socketId]);






    std::cout<<"Method: "<<_requests[socketId].GetMethod()<<std::endl;
    std::cout<<"Path: "<<_requests[socketId].GetPath()<<std::endl;
    std::cout<<"Version: "<<_requests[socketId].GetVersion()<<std::endl;
    _requests[socketId].printHeaders();
    std::cout<<"Body"<<std::endl;
    std::cout<<_requests[socketId].GetBody()<<std::endl;

    return true;
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