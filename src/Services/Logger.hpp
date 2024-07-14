#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <map>
#include <iostream>

#define Reset "\x1B[0m"
#define Red "\x1B[91m"
#define Yellow "\x1B[33m"
#define Blue "\x1B[94m"
#define Cyan "\x1B[36m"
#define GulfStandardTime 4;

enum LogPrio
{
    WARNING,
    INFO,
    DEBUG,
    ERROR
};

class Logger
{
    public:
        static int LogMsg(LogPrio prio, const char* output);
        static int LogMsg(LogPrio prio, const char* output, int socketId);

    private:
        static std::string GetCurrTime(void);
        static std::map<const char*, std::string> SetSetting(LogPrio prio);
};

#endif