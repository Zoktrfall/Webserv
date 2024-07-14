#include "Logger.hpp"

std::string Logger::GetCurrTime(void)
{
    tzset();
    char date[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    tm.tm_hour = tm.tm_hour + GulfStandardTime;
    strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]   ", &tm);
    return (std::string(date));
}
const char* Logger::SetColor(LogPrio prio)
{
    const char *color = NULL;
    if(prio == WARNING)
        color = Yellow;
    else if(prio == INFO)
        color = Cyan;
    else if(prio == ERROR)
        color = Red;
    else if(prio == DEBUG)
        color = Blue;
    return color;
}
int Logger::LogMsg(LogPrio prio, const char* output)
{
    const char *color = SetColor(prio);
    std::cout<<color<< GetCurrTime()<<output<<Reset<<std::endl;
    return 1;
}
