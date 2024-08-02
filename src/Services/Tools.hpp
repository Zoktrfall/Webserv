#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <string>
#include <cstring>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include "Logger.hpp"
#include "ServerDataExc.hpp"
#include "HttpRequestController.hpp"
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
        static void CheckRootOrLocation(std::string& dir, std::string ex, std::string error);
        static ThereIs PathExists(const std::string path);
        static std::string GetCurrentDateTime(void);
        static bool IsHidden(const std::string& filename);
        static bool EndsWith(const std::string& str, const std::string& suffix);
        static std::string GenerateHtmlFromDirectory(const std::string& dirPath, int socketId);

    private:
};

#endif