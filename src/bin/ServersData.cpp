#include "ServersData.hpp"

ServersData::ServersData(const char* configFilePath) : _configFile(configFilePath) {}

bool ServersData::SetupServersData(void)
{
    try
    {
        _content = _configFile.ProcessConfigFile();
        std::cout<<_content<<std::endl;
    }
    catch(const ServerDataExc& exp)
    {
        std::cout<<exp.what()<<std::endl;
        return false;
    }
    return true;
}