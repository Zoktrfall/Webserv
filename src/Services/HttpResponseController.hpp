#ifndef HTTP_RESPONSE_CONTROLLER_HPP
#define HTTP_RESPONSE_CONTROLLER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Tools.hpp"
#include "IHttpResponseController.hpp"
#include "HttpRequestController.hpp"
#include "ServersData.hpp"
#include "HttpTypes.hpp"

class HttpResponseController : public IHttpResponseController
{
    public :
        void HttpResponse(int socketId, Server& Server, Request& Request);

    private :
        Request _request;

};

#endif