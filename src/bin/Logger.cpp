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
std::map<const char*, std::string> Logger::SetSetting(LogPrio prio)
{
    std::map<const char*, std::string> setting;
    if(prio == WARNING)
        setting[Yellow] = "[WARNING]  ";
    else if(prio == INFO)
        setting[Cyan] = "[INFO]  ";
    else if(prio == ERROR)
        setting[Red] = "[ERROR]  ";
    else if(prio == DEBUG)
        setting[Blue] = "[DEBUG]  ";
    return setting;
}
int Logger::LogMsg(LogPrio prio, const char* output)
{
    std::map<const char*, std::string> setting = SetSetting(prio);
    std::map<const char*, std::string>::reverse_iterator rit = setting.rbegin();
    std::cout<<rit->first<<GetCurrTime()<<rit->second<<output<<Reset<<std::endl;
    return 1;
}
int Logger::LogMsg(LogPrio prio, const char* output, int socketId)
{
    std::map<const char*, std::string> setting = SetSetting(prio);
    std::map<const char*, std::string>::reverse_iterator rit = setting.rbegin();
    std::cout<<rit->first<<GetCurrTime()<<rit->second<<output<<" [SocketID:"<<socketId<<"]"<<Reset<<std::endl;
    return 1;
}
