#ifndef HTTP_SERVERS_HPP
#define HTTP_SERVERS_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "ServersData.hpp"
#include "ISocketsController.hpp"
#include "IWebServerController.hpp"
#include "HttpController.hpp"

struct ServerSocket
{
    struct sockaddr_in ServerAddress;
    int serverSocket;
};

struct ClientSocket
{
    int clientSocket;
    bool IsChunked;
};

class WebServer : public HttpController, IWebServerController, ISocketsController
{
    public : 
        WebServer(char* configFilePath);
        void RunWebServer(void);
    private : 
        ServersData _serversData;
        std::vector<ServerSocket> _serverSockets;
        std::vector<ClientSocket> _readSockets;
        std::vector<int> _writeSockets;
        int _maxAvailableFD;

        void CreateServer(void);
        void StartServer(void);

        void WriteSockets(fd_set& WriteFDS);
        void ReadSockets(fd_set& ReadFDS);
        void ServerSockets(fd_set& WriteFDS);
        void InitializeFDSets(fd_set& ReadFDS, fd_set& WriteFDS);
};

#endif