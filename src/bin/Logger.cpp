#include "Logger.hpp"

std::string Logger::getCurrTime(void)
{
    tzset();
    char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    tm.tm_hour = tm.tm_hour + GulfStandardTime;
    strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]   ", &tm);
    return (std::string(date));
}
