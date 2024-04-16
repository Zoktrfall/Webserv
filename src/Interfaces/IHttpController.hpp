#ifndef I_HTTP_CONTROLLER_HPP
#define I_HTTP_CONTROLLER_HPP

class IHttpController
{
    public :
        virtual bool HttpRequest(int) = 0;
        virtual void HttpResponse(int) = 0;  
        virtual ~IHttpController() = 0;
};

#endif