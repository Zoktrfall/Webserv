#include "WebServer.hpp"

WebServer::WebServer(char* configFilePath) : _serversData(configFilePath)
{
    // if(_serversData.IsOkay())
        // exit(0); // at this stage so far  
}

void WebServer::RunWebServer()
{
    WebServer::CreateServer();
    WebServer::StartServer();
}

void WebServer::CreateServer(void) //* Needs some work *
{
    /* Needs improvement, this is hard code */
    for (int i = 0; i < 5 /*_serversData.Lenght()*/ ; ++i) 
    {
        int port = 9090 + i;
        ServerSocket* server = new ServerSocket();;
        server->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        server->ServerAddress.sin_family = AF_INET;
        server->ServerAddress.sin_port = htons(port);
        _serverSockets.push_back(*server);
    }
    for(int i = 0; i < 5; i++)
        bind(_serverSockets[i].serverSocket, (struct sockaddr*)&_serverSockets[i].ServerAddress, sizeof(_serverSockets[i].ServerAddress));

    for(int i = 0; i < 5; i++)
        listen(_serverSockets[i].serverSocket, 1);
}

void WebServer::StartServer(void)
{
    fd_set	ReadFDS, WriteFDS;
    while(true)
    {
        int maxAvailableFD = BindFDS(ReadFDS, WriteFDS);

        // Socket State
        std::cout<<"ServerSockets: "<<_serverSockets.size()<<std::endl;
        std::cout<<"ReadSockets: "<<_readSockets.size()<<std::endl;
        std::cout<<"WriteSockets: "<<_writeSockets.size()<<std::endl;

        if(select(maxAvailableFD + 1, &ReadFDS, &WriteFDS, 0, 0) >= 0)
        {
            WebServer::WriteSockets(WriteFDS);
            WebServer::ReadSockets(ReadFDS);
            WebServer::ServerSockets(ReadFDS);   
        }
    }
}

void WebServer::WriteSockets(fd_set& WriteFDS)
{
    std::cout<<"->WriteSockets_FUNC"<<std::endl;
    for(size_t i = 0; i < _writeSockets.size(); ++i)
        if(FD_ISSET(_writeSockets[i], &WriteFDS))
        {
            std::cout<<"-->Response is Ready"<<std::endl;
            // call HttpResponse func...
            close(_writeSockets[i]);
            _writeSockets.erase(_writeSockets.begin() + i);
        }
       
}

void WebServer::ReadSockets(fd_set& ReadFDS)
{
    std::cout<<"->ReadSockets_FUNC"<<std::endl;
    for(size_t i = 0; i < _readSockets.size(); ++i)
        if(FD_ISSET(_readSockets[i], &ReadFDS))
        {
            // if(httpRequest is true)
                std::cout<<"-->Request is Ready"<<std::endl;
                _writeSockets.push_back(_readSockets[i]);
                _readSockets.erase(_readSockets.begin() + i);
            // else
                // std::cout<<"-->Request isn't Ready";
        }
}

void WebServer::ServerSockets(fd_set& ReadFDS)
{
    std::cout<<"->ServerSockets_FUNC"<<std::endl;
    for(size_t i = 0; i < _serverSockets.size(); ++i)
        if(FD_ISSET(_serverSockets[i].serverSocket, &ReadFDS))
        {
            int clientSocket = accept(_serverSockets[i].serverSocket, NULL, NULL);
            if (clientSocket != -1)
            {
                std::cout<<"-->Accepted new connection on socket "<<std::endl;
                _readSockets.push_back(clientSocket);
            }
            else
                std::cout<<"-->Error accepting new connection"<<std::endl;
        }
}

int WebServer::BindFDS(fd_set& ReadFDS, fd_set& WriteFDS)
{
    FD_ZERO(&WriteFDS);
    FD_ZERO(&ReadFDS);
    int maxAvailableFD = 3;

    for(size_t i = 0; i < _serverSockets.size(); i++)
    {
        if(_serverSockets[i].serverSocket > maxAvailableFD)
            maxAvailableFD = _serverSockets[i].serverSocket;
        FD_SET(_serverSockets[i].serverSocket, &ReadFDS);
    }

    for(size_t i = 0; i< _readSockets.size(); i++)
    {
        if(_readSockets[i] > maxAvailableFD)
            maxAvailableFD = _readSockets[i];
        FD_SET(_readSockets[i], &ReadFDS);
    }

    for(size_t i = 0; i < _writeSockets.size(); i++)
    {
        if(_writeSockets[i] > maxAvailableFD)
            maxAvailableFD = _writeSockets[i];
        FD_SET(_writeSockets[i], &WriteFDS);
    }

    return maxAvailableFD;
}