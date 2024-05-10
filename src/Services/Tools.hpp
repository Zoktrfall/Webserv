#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <string>
#include <sys/socket.h>

#define WhiteSpaces " \n\t\v\f\r"

class Tools
{
    public :
        static std::string ToLower(const std::string& str);
        static std::string& Trim(std::string& str, const std::string& trimmerStr);
        static std::string Recv(const int socketId, const int buffer);
    private:
};

#endif