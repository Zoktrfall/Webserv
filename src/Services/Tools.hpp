#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ServerDataExc.hpp"
#include "HttpController.hpp"
#include "HttpTypes.hpp"

#define WhiteSpaces " \n\t\v\f\r"

class Tools
{
    public :
        static bool IsWhiteSpace(const std::string& s);
        static bool IsUpperCase(const std::string& str);
        static std::string ToLower(const std::string& str);
        static std::string& Trim(std::string& str, const std::string& trimmerStr);
        static RequestResult Recv(int socketId, char* requestBuffer, int& bytesRead);
        static std::string ToString(int value);
        static bool IsAllDigits(const std::string& str);
        static void CheckSemicolon(std::string& configPart);
        static unsigned long StrToULong(const std::string& str);
        static void Stat(const std::string& path, int mode, std::string messege);
        static unsigned long long CheckClientMaxBodySize(std::string ClientSize);
        static int CheckAutoIndex(std::string autoIndex);
        static std::vector<std::string> CheckIndices(std::string indices);
        static void AccessStat(int mode, int accMode, std::string error, std::string value);
        static std::map<int, std::string> InitReturn(std::string ret);

    private:
};

#endif