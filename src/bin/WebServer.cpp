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
        if(select(_maxAvailableFD + 1, &ReadFDS, &WriteFDS, NULL, &timer) < 0)
        {
            if(errno != EPIPE)
            {
                std::cout<<strerror(errno)<<std::endl;
                exit(1);
            }
            std::cout<<"error"<<std::endl;
            continue;
        }
        WebServer::WriteSockets(WriteFDS);
        WebServer::ReadSockets(ReadFDS);
        WebServer::ServerSockets(ReadFDS);   
        CheckTimeout();
    }
}

void WebServer::WriteSockets(fd_set& WriteFDS)
{
    for(size_t i = 0; i < _writeSockets.size(); ++i)
        if(FD_ISSET(_writeSockets[i], &WriteFDS))
        {
            HttpController::HttpResponse(_writeSockets[i]);
            CloseConnection(_writeSockets, i);
        }
}

void WebServer::ReadSockets(fd_set& ReadFDS)
{
    for(size_t i = 0; i < _readSockets.size(); ++i)
        if(FD_ISSET(_readSockets[i].clientSocket, &ReadFDS) || _readSockets[i].IsChunked)
        {
            RequestResult requestResult = HttpController::HttpRequest(_readSockets[i].clientSocket);
            if(requestResult == ClosedConnection)
            {
                std::cout<<"Client Closed Connection"<<std::endl;
                return CloseConnection(_readSockets, i);
            }
            else if(requestResult == ReadError)
            {
                std::cout<<"Read Error"<<std::endl;
                return CloseConnection(_readSockets, i);
            }
            else if(requestResult == Chunked)
                _readSockets[i].IsChunked = true;
            else
                MoveSocketFromReadToWrite(i);
        }
}

void WebServer::ServerSockets(fd_set& ReadFDS)
{
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

                ClientSocket newClient;
                newClient.clientSocket = clientSocket;
                newClient.IsChunked = false;
                newClient.lastTime = time(NULL);
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

    for(size_t i = 0; i < _readSockets.size(); ++i)
    {
        if(_readSockets[i].clientSocket > _maxAvailableFD)
            _maxAvailableFD = _readSockets[i].clientSocket;
        FD_SET(_readSockets[i].clientSocket, &ReadFDS);
    }

    for(size_t i = 0; i < _serverSockets.size(); ++i)
    {
        if(_serverSockets[i].serverSocket > _maxAvailableFD)
            _maxAvailableFD = _serverSockets[i].serverSocket;
        FD_SET(_serverSockets[i].serverSocket, &ReadFDS);
    }

    for(size_t i = 0; i < _writeSockets.size(); ++i)
    {
        if(_writeSockets[i] > _maxAvailableFD)
            _maxAvailableFD = _writeSockets[i];
        FD_SET(_writeSockets[i], &WriteFDS);
    }
}

void WebServer::CheckTimeout(void)
{
    for(size_t i = 0; i < _readSockets.size(); ++i)
        if(time(NULL) - _readSockets[i].lastTime > 60)
        {
            
            CloseConnection(_readSockets, i);
        }
}

void WebServer::CloseConnection(std::vector<ClientSocket>& sockets, int index) 
{
    close(sockets[index].clientSocket);
    sockets.erase(sockets.begin() + index);
}

void WebServer::CloseConnection(std::vector<int>& sockets, int index) 
{
    close(sockets[index]);
    sockets.erase(sockets.begin() + index);
}

void WebServer::MoveSocketFromReadToWrite(int index)
{
    _writeSockets.push_back(_readSockets[index].clientSocket);
    _readSockets.erase(_readSockets.begin() + index);
}