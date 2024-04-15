#include <iostream>
#include <string>
#include "WebServer.hpp"

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cout<<"Too many arguments provided."<<std::endl;
        return 0;
    }   
        
    WebServer mainServer(argv[1]);
    mainServer.RunWebServer();
    // system("leaks webserv");
    return 0;
}