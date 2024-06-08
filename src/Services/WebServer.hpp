#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <signal.h>
#include "ServersData.hpp"
#include "ISocketsController.hpp"
#include "IWebServerController.hpp"
#include "HttpController.hpp"
#include "HttpTypes.hpp"
#include "Logger.hpp"

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

        void InitializeData(void);
        void SetupServer(void);
        void StartServer(void);

        void WriteSockets(fd_set& WriteFDS);
        void ReadSockets(fd_set& ReadFDS);
        void ServerSockets(fd_set& WriteFDS);
        void InitializeFDSets(fd_set& ReadFDS, fd_set& WriteFDS);

        void CheckTimeout(void);

        void CloseConnection(std::vector<ClientSocket>& sockets, int index);
        void CloseConnection(std::vector<int>& sockets, int index);
        void MoveSocketFromReadToWrite(int index);
};

#endif