#ifndef HTTP_CONTROLLER_HPP
#define HTTP_CONTROLLER_HPP
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <vector>
#include "Request.hpp"

// Default
# define RECV_SIZE 4096
// # define CGI_BUFSIZE 4096

class HttpController
{
    public :
        bool HttpRequest(int readSocket);
        void HttpResponse(int writeSocket);

        virtual ~HttpController() {};
    private :
        std::map<int, Request> _requests;

        bool CheckRequestIn(int sokcetId);
        void CreateNewRequest(int socketId);
        bool ReadRequest(int socketId);
        bool ParseRequest(std::string& requestContent, Request& request);
};

#endif