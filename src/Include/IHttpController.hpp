#ifndef I_HTTP_CONTROLLER_HPP
#define I_HTTP_CONTROLLER_HPP
#include "HttpTypes.hpp"

class IHttpController
{
    public :
        virtual ~IHttpController() {};
        virtual RequestResult HttpRequest(int) = 0;
        virtual void HttpResponse(int) = 0;  
};

#endif