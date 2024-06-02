#include "WebServer.hpp"

void sigpipeHandle(int sig) { if(sig){} }
int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cout<<"Too many arguments provided."<<std::endl;
        return 0;
    }   
        
    signal(SIGPIPE, sigpipeHandle);
    WebServer mainServer(argv[1]);
    mainServer.RunWebServer();
    return 0;
}