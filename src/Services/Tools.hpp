#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <string>
#include <cstring>
#include "IHttpController.hpp"

#define WhiteSpaces " \n\t\v\f\r"
#define RecvSize 4096
#define LimitRequestBody 65536
#define CGIBufSize 4096

class Tools
{
    public :
        static std::string ToLower(const std::string& str);
        static std::string& Trim(std::string& str, const std::string& trimmerStr);
        static RequestResult Recv(int socketId, char* requestBuffer, size_t bufferSize);
    private:
};

#endif