#ifndef I_HTTP_CONTROLLER_HPP
#define I_HTTP_CONTROLLER_HPP
#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <map>

class IHttpController
{
    public :
        virtual ~IHttpController() {};
        virtual bool HttpRequest(int) = 0;
        virtual void HttpResponse(int) = 0;  
};

#endif