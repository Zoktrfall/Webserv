#include "ServersData.hpp"

ServersData::ServersData(const char* configFilePath) : _configFile(configFilePath) {}

bool ServersData::SetupServersData(void)  //* Needs some work *
{
    try
    {
        _content = _configFile.ProcessConfigFile();

        ConfigParser::removeComments(Tools::Trim(_content, WhiteSpaces));
        ConfigParser::parseServerBlocks(_content);

    }
    catch(const ServerDataExc& exp)
    {
        std::cout<<exp.what()<<std::endl;
        return false;
    }
    return true;
}