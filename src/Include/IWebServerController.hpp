#ifndef I_WEB_SERVER_CONTROLLER_HPP
#define I_WEB_SERVER_CONTROLLER_HPP

class IWebServerController
{
    public :
        virtual ~IWebServerController() {};
        virtual void RunWebServer() = 0;
        virtual void SetupServer(void) = 0;
        virtual void StartServer(void) = 0;
};

#endif