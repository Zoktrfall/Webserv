#include "HttpController.hpp"
// #include <fstream>


bool HttpController::CheckRequestIn(int socketId) { return _requests.count(socketId) > 0 ? true : false; }
void HttpController::CreateNewRequest(int socketId) { _requests[socketId] = Request(); }

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
    // std::cout<<std::endl;
    // std::cout<<line<<std::endl;
    // std::cout<<std::endl;


    std::istringstream iss(line);
    std::string headerName, headerValue;
    std::getline(iss, headerName, ':');
    std::getline(iss, headerValue);


    // headerValue = Tools::ToLower(Tools::Trim(headerValue, WhiteSpaces));
    
    // std::cout<<"headerName:"<<headerName<<std::endl;
    // std::cout<<"headerValue:"<<headerValue<<std::endl;

    request.SetHeader(headerName, Tools::ToLower(Tools::Trim(headerValue, WhiteSpaces)));
}

bool HttpController::ParseHTTPRequest(std::string& requestContent, Request& request)
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

    return true;       
}

bool HttpController::ProcessHTTPRequest(int socketId)
{
    char requestBuffer[RECV_SIZE];
    int readBytes;
    std::string requestContent = "";
    while(true)
    {
        bzero(&requestBuffer, RECV_SIZE * sizeof(char));
        readBytes = recv(socketId, requestBuffer, sizeof(requestBuffer), 0);
        if (readBytes <= 0)
			break ;
        
        requestContent += std::string(requestBuffer);
        if(!ParseHTTPRequest(requestContent, _requests[socketId]))
            return false;


        // std::cout<<std::endl;
        // std::cout<<readBytes<<std::endl;
        // std::cout<<requestContent<<std::endl;
        // std::cout<<std::endl;


        // std::ofstream outputFile("request_content.txt");
        // outputFile << requestContent;
        // outputFile.close();
        // std::cout << "Содержимое запроса сохранено в файле 'request_content.txt'" << std::endl;
        
        
        break;
    }
    // _requests.erase(socketId); //* Needs some work *
    
    // std::cout<<std::endl;
    // std::cout<<_requests[socketId].GetMethod()<<std::endl;
    // std::cout<<_requests[socketId].GetPath()<<std::endl;
    // std::cout<<_requests[socketId].GetVersion()<<std::endl;
    // std::cout<<std::endl;

    return true;
}

bool HttpController::HttpRequest(int readSocket)
{   
    if(!CheckRequestIn(readSocket))
        CreateNewRequest(readSocket);
    for (std::map<int, Request>::iterator it = _requests.begin(); it != _requests.end(); ++it) {
            std::cout << "Key: " << it->first << ", Value: "<< it->second.GetMethod() <<" "<< it->second.GetPath() << " " << it->second.GetVersion() << std::endl;
            it->second.printHeaders();
    }
    return ProcessHTTPRequest(readSocket);
}

void HttpController::HttpResponse(int readSocket) //* Needs some work *
{
    const char *http_response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
    send(readSocket, http_response, strlen(http_response), 0);
}