#ifndef I_SOCKETS_CONTROLLER_HPP
#define I_SOCKETS_CONTROLLER_HPP
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <fcntl.h>

class ISocketsController
{
    public :
        virtual ~ISocketsController() {};
        virtual void InitializeFDSets(fd_set&, fd_set&) = 0;
        virtual void WriteSockets(fd_set&) = 0;
        virtual void ReadSockets(fd_set&) = 0;
        virtual void ServerSockets(fd_set&) = 0;
};

#endif