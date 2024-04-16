#ifndef HTTP_SERVERS_HPP
#define HTTP_SERVERS_HPP

#include <iostream>
#include <string>
#include <vector>
#include "ServersData.hpp"
#include "ISocketsController.hpp"
#include "IWebServerController.hpp"
#include "HttpController.hpp"

struct ServerSocket
{
    struct sockaddr_in ServerAddress;
    int serverSocket;
};

class WebServer : public HttpController, IWebServerController, ISocketsController
{
    public : 
        WebServer(char* configFilePath);
        void RunWebServer(void);
    private : 
        ServersData _serversData;
        std::vector<ServerSocket> _serverSockets;
        std::vector<int> _readSockets;
        std::vector<int> _writeSockets;

        void CreateServer(void);
        void StartServer(void);

        void WriteSockets(fd_set& WriteFDS);
        void ReadSockets(fd_set& ReadFDS);
        void ServerSockets(fd_set& WriteFDS);
        int BindFDS(fd_set& ReadFDS, fd_set& WriteFDS);
};

#endif