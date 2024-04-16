#ifndef HTTP_CONTROLLER_HPP
#define HTTP_CONTROLLER_HPP
#include "IHttpController.hpp"

class HttpController : public IHttpController
{
    public :
        bool HttpRequest(int readSocket);
        void HttpResponse(int writeSocket);

        virtual ~HttpController() {};
    private :
        
        
};

#endif