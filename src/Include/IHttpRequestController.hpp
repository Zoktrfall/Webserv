#ifndef I_HTTP_REQUEST_CONTROLLER_HPP
#define I_HTTP_REQUEST_CONTROLLER_HPP
#include "HttpTypes.hpp"

class IHttpRequestController
{
    public :
        virtual ~IHttpRequestController() {};
        virtual RequestResult HttpRequest(int) = 0; 
};

#endif