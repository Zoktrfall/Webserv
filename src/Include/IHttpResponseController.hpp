#ifndef I_HTTP_RESPONSE_CONTROLLER_HPP
#define I_HTTP_RESPONSE_CONTROLLER_HPP
#include "HttpTypes.hpp"
#include "HttpRequestController.hpp"
#include "Server.hpp"

class IHttpResponseController
{
    public :
        virtual ~IHttpResponseController() {};
        virtual void HttpResponse(int, Server&, Request&) = 0;
};

#endif