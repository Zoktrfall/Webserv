#include "HttpController.hpp"
// #include <fstream>


bool HttpController::CheckRequestIn(int socketId) { return _requests.count(socketId) > 0 ? true : false; }
void HttpController::CreateNewRequest(int socketId) { _requests[socketId] = Request(); }

bool HttpController::ParseRequest(std::string& requestContent, Request& request)
{
    std::vector<std::string> lines;
    std::string line;
    size_t pos = 0;
    while ((pos = requestContent.find("\r\n", pos)) != std::string::npos)
    {
        lines.push_back(requestContent.substr(0, pos));
        requestContent.erase(0, pos + 2);
    }

    for(int i = 0; i < lines.size(); i++)
        std::cout<<lines[i]<<std::endl;

    return true;       
}

bool HttpController::ReadRequest(int socketId)
{
    char requestBuffer[RECV_SIZE] = {0};
    int readBytes;
    std::string requestContent = "";
    while(true)
    {
        readBytes = recv(socketId, requestBuffer, sizeof(requestBuffer), 0);
        if (readBytes <= 0)
			break ;
        
        requestContent += std::string(requestBuffer);
        if(!ParseRequest(requestContent, _requests[socketId]))
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
    _requests.erase(socketId);
    return true;
}

bool HttpController::HttpRequest(int readSocket)
{
    if(!CheckRequestIn(readSocket))
        CreateNewRequest(readSocket);
    return ReadRequest(readSocket);
}

void HttpController::HttpResponse(int readSocket) //* Needs some work *
{
    const char *http_response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
    send(readSocket, http_response, strlen(http_response), 0);
}