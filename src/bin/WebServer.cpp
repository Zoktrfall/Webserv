#include "WebServer.hpp"

WebServer::WebServer(char* configFilePath) : _serversData(configFilePath)
{
    // if(_serversData.IsOkay())
        // exit(0); // at this stage so far  
}

void WebServer::Start()
{
    WebServer::CreateServer();
    WebServer::UpServer();
}


void WebServer::CreateServer(void)
{
    for (int i = 0; i < 5 /*_serversData.Lenght()*/ ; ++i)
        _serverSockets.push_back(socket(AF_INET, SOCK_STREAM, 0));
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET; 
    addr1.sin_port = htons(333);
    struct sockaddr_in addr2;
    addr2.sin_family = AF_INET; 
    addr2.sin_port = htons(222);
    struct sockaddr_in addr3;
    addr3.sin_family = AF_INET;
    addr3.sin_port = htons(312);
    struct sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(451);
    struct sockaddr_in addr5;
    addr5.sin_family = AF_INET;
    addr5.sin_port = htons(678);
    bind(_serverSockets[0], (struct sockaddr*)&addr1, sizeof(addr1));
    bind(_serverSockets[1], (struct sockaddr*)&addr2, sizeof(addr2));
    bind(_serverSockets[2], (struct sockaddr*)&addr3, sizeof(addr3));
    bind(_serverSockets[3], (struct sockaddr*)&addr4, sizeof(addr4));
    bind(_serverSockets[4], (struct sockaddr*)&addr5, sizeof(addr5));

    for(int i = 0; i < 5; i++)
        listen(_serverSockets[i], 1);
}

void WebServer::UpServer(void)
{
    fd_set	ReadFDS, WriteFDS;
    while(true)
    {
        int LastAvailableFD = BindFDS(ReadFDS, WriteFDS);
        std::cout<<_serverSockets.size()<<std::endl;
        std::cout<<_readSockets.size()<<std::endl;
        std::cout<<_writeSockets.size()<<std::endl;
        if(select(LastAvailableFD + 1, &ReadFDS, &WriteFDS, 0, 0) >= 0)
        {
            // WebServer::WriteSockets(WriteFDS);
            // WebServer::ReadSockets(ReadFDS);
            WebServer::ServerSockets(WriteFDS);   
        }
        break;
    }
}

void WebServer::WriteSockets(fd_set& WriteFDS)
{
    std::cout<<"WriteSockets"<<std::endl;
    for(size_t i = 0; i < _writeSockets.size(); ++i)
        if(FD_ISSET(_writeSockets[i], &WriteFDS))
        {
            std::cout<<"Response is Ready"<<std::endl;
            // call HttpResponse func...
            close(_writeSockets[i]);
            _writeSockets.erase(_writeSockets.begin() + i);
        }
       
}

void WebServer::ReadSockets(fd_set& ReadFDS)
{
    std::cout<<"ReadSockets"<<std::endl;
    for(size_t i = 0; i < _readSockets.size(); ++i)
        if(FD_ISSET(_readSockets[i], &ReadFDS))
        {
            std::cout<<"Request is Ready"<<std::endl;
            close(_readSockets[i]);
            _readSockets.erase(_readSockets.begin() + i);
        }
}

void WebServer::ServerSockets(fd_set& ReadFDS)
{
    std::cout<<"ServerSockets"<<std::endl;
    for(size_t i = 0; i < _serverSockets.size(); ++i)
        if(FD_ISSET(_serverSockets[i], &ReadFDS))
        {
            std::cout<<"Fd_set"<<std::endl;
            exit(0);
            int clientSocket = accept(_serverSockets[i], NULL, NULL);
            if (clientSocket != -1)
            {
                std::cout<<"Accepted new connection on socket "<<std::endl;
                _readSockets.push_back(clientSocket);
            }
            else
                std::cout<<"Error accepting new connection"<<std::endl;
        }
}



int WebServer::BindFDS(fd_set& ReadFDS, fd_set& WriteFDS)
{
    FD_ZERO(&WriteFDS);
    FD_ZERO(&ReadFDS);
    int LastAvailableFD = 3;

    for(size_t i = 0; i < _serverSockets.size(); i++)
    {
        if(_serverSockets[i] > LastAvailableFD)
            LastAvailableFD = _serverSockets[i];
        FD_SET(_serverSockets[i], &ReadFDS);
    }

    for(size_t i = 0; i< _readSockets.size(); i++)
    {
        if(_readSockets[i] > LastAvailableFD)
            LastAvailableFD = _readSockets[i];
        FD_SET(_readSockets[i], &ReadFDS);
    }

    for(size_t i = 0; i < _writeSockets.size(); i++)
    {
        if(_writeSockets[i] > LastAvailableFD)
            LastAvailableFD = _writeSockets[i];
        FD_SET(_writeSockets[i], &WriteFDS);
    }

    return LastAvailableFD;
}