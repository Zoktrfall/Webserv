#include "WebServer.hpp"

void SigPipeHandle(int sig) { if(sig){ Logger::LogMsg(WARNING, "Caught SIGPIPE signal");} }
int main(int argc, char* argv[])
{
    if(argc > 2)
        Logger::LogMsg(ERROR, "Too many arguments provided.");
    else
    {
        signal(SIGPIPE, SigPipeHandle);
        WebServer mainServer(argv[1]);
        mainServer.RunWebServer();
    }
    return 0;
}