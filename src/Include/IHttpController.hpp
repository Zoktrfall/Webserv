#ifndef I_HTTP_CONTROLLER_HPP
#define I_HTTP_CONTROLLER_HPP
#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <map>
#include "HttpTypes.hpp"


class IHttpController
{
    public :
        virtual ~IHttpController() {};
        virtual RequestResult HttpRequest(int) = 0;
        virtual void HttpResponse(int) = 0;  
};

#endif