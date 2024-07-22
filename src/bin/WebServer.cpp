#include "WebServer.hpp"

WebServer::WebServer(char* configFilePath) : _serversData(configFilePath) {}
void WebServer::RunWebServer()
{
    WebServer::SetupServer();
    WebServer::StartServer();
}
void WebServer::SetupServer(void)
{
    Logger::LogMsg(INFO, "Reading and checking the configuration file");
    if(!_serversData.SetupServersData())
        exit(1);

    Logger::LogMsg(INFO, "Initializing  Servers");
    for(size_t i = 0; i < _serversData.GetServers().size(); ++i)
    {
        std::vector<uint16_t> ports = _serversData.GetServers()[i].GetPorts();
        std::vector<in_addr_t> hosts = _serversData.GetServers()[i].GetHosts();
        for(size_t j = 0; j < ports.size() && j < hosts.size(); ++j)
        {
            ServerSocket Server;
            memset(&Server.serverAddress, 0, sizeof(Server.serverAddress));
            Server.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if(Server.serverSocket == -1)
                exit(Logger::LogMsg(ERROR, strerror(errno)));

            int optionValue = 1;
	        if(setsockopt(Server.serverSocket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(int)) == -1)
                exit(Logger::LogMsg(ERROR, strerror(errno)));

            Server.serverAddress.sin_family = AF_INET;
            Server.serverAddress.sin_addr.s_addr = hosts[j];
            Server.serverAddress.sin_port = htons(ports[j]);

            if(bind(Server.serverSocket, (struct sockaddr*)&Server.serverAddress, 
                sizeof(Server.serverAddress)) == -1)
                exit(Logger::LogMsg(ERROR, strerror(errno)));

            if(fcntl(Server.serverSocket, F_SETFL, O_NONBLOCK) < 0)
                exit(Logger::LogMsg(ERROR, strerror(errno)));

            if(listen(Server.serverSocket, 1) == -1)
                exit(Logger::LogMsg(ERROR, strerror(errno)));

            Server.serverIndex = i;
            _serverSockets.push_back(Server);
        }
    }
}
void WebServer::StartServer(void)
{
    Logger::LogMsg(INFO, "Starting Miracle WebServer");
    fd_set	ReadFDS, WriteFDS;
    struct timeval timer;

    while(true)
    {
        timer.tv_sec = 1;
        timer.tv_usec = 0;
        InitializeFDSets(ReadFDS, WriteFDS);
        if(select(_maxAvailableFD + 1, &ReadFDS, &WriteFDS, NULL, &timer) < 0)
            exit(Logger::LogMsg(ERROR, strerror(errno)));

        WebServer::ServerSockets(ReadFDS);   
        WebServer::ReadSockets(ReadFDS);
        WebServer::WriteSockets(WriteFDS);
        CheckTimeout();
    }
}
void WebServer::WriteSockets(fd_set& WriteFDS)
{
    for(size_t i = 0; i < _writeSockets.size(); ++i)
        if(FD_ISSET(_writeSockets[i].responseSocket, &WriteFDS))
        {
            _ResponseController.HttpResponse(_writeSockets[i].responseSocket,
                                        _serversData.GetServers()[_writeSockets[i].serverIndex],
                                        HttpRequestController::GetRequest(_writeSockets[i].responseSocket));
            Logger::LogMsg(DEBUG, "Response Sent To Socket", _writeSockets[i].responseSocket);
            HttpRequestController::ClearRequest(_writeSockets[i].responseSocket);
            CloseConnection(_writeSockets, i);
        }
}
void WebServer::ReadSockets(fd_set& ReadFDS)
{
    for(size_t i = 0; i < _readSockets.size(); ++i)
        if(FD_ISSET(_readSockets[i].clientSocket, &ReadFDS))
        {
            RequestResult requestResult = HttpRequestController::HttpRequest(_readSockets[i].clientSocket);
            if(requestResult == ClosedConnection)
            {
                Logger::LogMsg(DEBUG, "Client Closed Connection", _readSockets[i].clientSocket);
                return CloseConnection(_readSockets, i);
            }
            else if(requestResult == ReadError)
            {
                Logger::LogMsg(WARNING, "Error reading from socket", _readSockets[i].clientSocket);
                return CloseConnection(_readSockets, i);
            }
            else if(requestResult == Multipart || requestResult == Chunked)
                _readSockets[i].lastTime = time(NULL);
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
            if(clientSocket != -1)
            {
                if(fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
                {
                    Logger::LogMsg(WARNING, "Failed to get flags for client socket",  _readSockets[i].clientSocket);
                    close(clientSocket);
                    return ;
                }

                ClientSocket newClient;
                newClient.serverIndex = _serverSockets[i].serverIndex;
                newClient.clientSocket = clientSocket;
                newClient.lastTime = time(NULL);
                _readSockets.push_back(newClient);
                Logger::LogMsg(DEBUG, "Connection accepted", clientSocket);
            }
            else
                Logger::LogMsg(WARNING, "Failed to accept connection",  _serverSockets[i].serverSocket);
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
        if(_writeSockets[i].responseSocket > _maxAvailableFD)
            _maxAvailableFD = _writeSockets[i].responseSocket;
        FD_SET(_writeSockets[i].responseSocket, &WriteFDS);
    }
}
void WebServer::CheckTimeout(void)
{
    for(size_t i = 0; i < _readSockets.size(); ++i)
        if(time(NULL) - _readSockets[i].lastTime > ConnectionTemeOut)
        {
            Logger::LogMsg(WARNING, "Client Timeout, Closing Connection..", _readSockets[i].clientSocket);
            CloseConnection(_readSockets, i);
        }
}
void WebServer::CloseConnection(std::vector<ClientSocket>& sockets, int index) 
{
    Logger::LogMsg(DEBUG, "Connection Closed.",  sockets[index].clientSocket);
    close(sockets[index].clientSocket);
    sockets.erase(sockets.begin() + index);

}
void WebServer::CloseConnection(std::vector<ResponseSocket>& sockets, int index) 
{
    Logger::LogMsg(DEBUG, "Connection Closed.",  sockets[index].responseSocket);
    close(sockets[index].responseSocket);
    sockets.erase(sockets.begin() + index);
}
void WebServer::MoveSocketFromReadToWrite(int index)
{
    ResponseSocket newResponse;
    newResponse.serverIndex = _readSockets[index].serverIndex;
    newResponse.responseSocket = _readSockets[index].clientSocket;

    _writeSockets.push_back(newResponse);
    _readSockets.erase(_readSockets.begin() + index);
}