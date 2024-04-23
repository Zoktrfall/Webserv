#ifndef I_HTTP_CONTROLLER_HPP
#define I_HTTP_CONTROLLER_HPP
#include <iostream>
#include <vector>
#include <map>

class IHttpController
{
    public :
        virtual bool HttpRequest(int) = 0;
        virtual void HttpResponse(int) = 0;  
        virtual ~IHttpController() = 0;
};

#endif