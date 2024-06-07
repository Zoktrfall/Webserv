#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>

#define Reset "\x1B[0m"
#define Red "\x1B[31m"
#define LightRed "\x1B[91m"
#define White "\x1B[37m"
#define Blink "\x1b[5m"
#define Yellow "\x1B[33m"
#define LightBlue "\x1B[94m"
#define Cyan "\x1B[36m"
#define DarkGrey "\x1B[90m"
#define LightMagenta "\x1B[95m"
#define GulfStandardTime 4;

enum LogPrio{
    DEBUG,
    INFO,
    ERROR
};

class Logger
{
    public:
        static void logMsg();
    private:
        static std::string getCurrTime(void);

};

#endif