#include "WebServer.hpp"

WebServer::WebServer(char* configFilePath) : _serversData(configFilePath)
{
    // if(!_serversData.IsOkay()) // //* Parts some work *
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


        ServerSocket* server = new ServerSocket();
        memset(&server->ServerAddress, 0, sizeof(server->ServerAddress));
        server->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(server->serverSocket <= -1)
        {
            std::cout<<"Error in <socket>"<<std::endl;
            exit(0);
        }

        int optionValue = 1;
	    if (setsockopt(server->serverSocket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(int)) < 0)
		    exit(1);
    
        server->ServerAddress.sin_family = AF_INET;
        server->ServerAddress.sin_addr.s_addr = INADDR_ANY; // Need to Improve
        server->ServerAddress.sin_port = htons(port);
        _serverSockets.push_back(*server);
    }


    for(int i = 0; i < 5; i++)
        if(bind(_serverSockets[i].serverSocket, (struct sockaddr*)&_serverSockets[i].ServerAddress, 
            sizeof(_serverSockets[i].ServerAddress)) == -1)
        {
            std::cout<<"Error in <bind>"<<std::endl;
            exit(1);
        }


    for(int i = 0; i < 5; i++)
    {
        if(fcntl(_serverSockets[i].serverSocket, F_SETFL, O_NONBLOCK) < 0)
        {   
            std::cout<<"Error in fcntl"<<std::endl;
            exit(1);
        }

        if(listen(_serverSockets[i].serverSocket, 1) == -1)
        {
            std::cout<<"Error in <listen>"<<std::endl;
            exit(1);
        }
    }
}




void WebServer::StartServer(void)
{
    fd_set	ReadFDS, WriteFDS;
    struct timeval timer;

    while(true)
    {
        timer.tv_sec = 1;
        timer.tv_usec = 0;
        InitializeFDSets(ReadFDS, WriteFDS);
        
        // Socket State
        std::cout<<"ServerSockets: "<<_serverSockets.size()<<std::endl;
        std::cout<<"ReadSockets: "<<_readSockets.size()<<std::endl;
        std::cout<<"WriteSockets: "<<_writeSockets.size()<<std::endl;

        //  select(_maxAvailableFD + 1, &ReadFDS, &WriteFDS, NULL, NULL) //* Needs some work *
        if(select(_maxAvailableFD + 1, &ReadFDS, &WriteFDS, NULL, &timer) >= 0)
        {
            WebServer::WriteSockets(WriteFDS);
            WebServer::ReadSockets(ReadFDS);
            WebServer::ServerSockets(ReadFDS);   
        }
        else
        {
            std::cout<<"Error in Select"<<std::endl;
            exit(1);
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
            HttpController::HttpResponse(_writeSockets[i]);
            close(_writeSockets[i]);
            _writeSockets.erase(_writeSockets.begin() + i);
        }
}

void WebServer::ReadSockets(fd_set& ReadFDS)
{
    std::cout<<"->ReadSockets_FUNC"<<std::endl;
    for(size_t i = 0; i < _readSockets.size(); ++i)
        if(FD_ISSET(_readSockets[i].clientSocket, &ReadFDS) || _readSockets[i].IsChunked)
        {
            if(HttpController::HttpRequest(_readSockets[i].clientSocket))
            {
                std::cout<<"-->Request is Ready"<<std::endl;
                _writeSockets.push_back(_readSockets[i].clientSocket);
                _readSockets.erase(_readSockets.begin() + i);
            }
            else
            {
                _readSockets[i].IsChunked = true;
                std::cout<<"-->Request isn't Ready"<<std::endl;
            }
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
                if(fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0)
                {
                    std::cout<<"-->Error accepting new connection"<<std::endl;
                    close(clientSocket);
                    return ;
                }

                std::cout<<"-->Accepted new connection on socket "<<std::endl;
                ClientSocket newClient;
                newClient.clientSocket = clientSocket;
                newClient.IsChunked = false;
                _readSockets.push_back(newClient);
            }
            else
                std::cout<<"-->Error accepting new connection"<<std::endl;
        }
}

void WebServer::InitializeFDSets(fd_set& ReadFDS, fd_set& WriteFDS)
{
    FD_ZERO(&WriteFDS);
    FD_ZERO(&ReadFDS);
    _maxAvailableFD = 0;

    for(size_t i = 0; i < _readSockets.size(); i++)
    {
        if(_readSockets[i].clientSocket > _maxAvailableFD)
            _maxAvailableFD = _readSockets[i].clientSocket;
        FD_SET(_readSockets[i].clientSocket, &ReadFDS);
    }

    for(size_t i = 0; i < _serverSockets.size(); i++)
    {
        if(_serverSockets[i].serverSocket > _maxAvailableFD)
            _maxAvailableFD = _serverSockets[i].serverSocket;
        FD_SET(_serverSockets[i].serverSocket, &ReadFDS);
    }

    for(size_t i = 0; i < _writeSockets.size(); i++)
    {
        if(_writeSockets[i] > _maxAvailableFD)
            _maxAvailableFD = _writeSockets[i];
        FD_SET(_writeSockets[i], &WriteFDS);
    }
}