#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define LIGHT_RED "\x1B[91m"
#define WHITE "\x1B[37m"
#define BLINK "\x1b[5m"
#define YELLOW "\x1B[33m"
#define LIGHT_BLUE "\x1B[94m"
#define CYAN "\x1B[36m"
#define DARK_GREY "\x1B[90m"
#define LIGHTMAGENTA "\x1B[95m"
#define GulfStandardTime 4;

class Logger
{
    public:
        static void logMsg();
    private:
        static std::string getCurrTime(void);

};

#endif