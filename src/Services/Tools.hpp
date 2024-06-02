#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <string>
#include <cstring>
#include "HttpTypes.hpp"

#define WhiteSpaces " \n\t\v\f\r"

class Tools
{
    public :
        static std::string ToLower(const std::string& str);
        static std::string& Trim(std::string& str, const std::string& trimmerStr);
        static RequestResult Recv(int socketId, char* requestBuffer, size_t bufferSize);
    private:
};

#endif