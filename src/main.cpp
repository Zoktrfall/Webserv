#include "WebServer.hpp"

void SigPipeHandle(int sig) { if(sig){} }
int main(int argc, char* argv[])
{
    if(argc > 2)
        std::cout<<"Too many arguments provided."<<std::endl;
    else
    {
        signal(SIGPIPE, SigPipeHandle);
        WebServer mainServer(argv[1]);
        mainServer.RunWebServer();
    }
    return 0;
}