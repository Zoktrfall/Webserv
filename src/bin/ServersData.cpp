#include "ServersData.hpp"

ServersData::ServersData(const char* configFilePath) : _configFile(configFilePath), _content("") {}

bool ServersData::SetupServersData(void)
{
    try
    {
        _content = _configFile.ProcessConfigFile();
        ConfigParser::removeComments(Tools::Trim(_content, WhiteSpaces));
        ConfigParser::parseServerBlocks(_content);
        _content.clear();
        ServersData::LoadServers();

    }
    catch(const ServerDataExc& exp)
    {
        std::cout<<exp.what()<<std::endl;
        return false;
    }
    return true;
}

void ServersData::LoadServers(void)
{
    for(size_t i = 0; i < ConfigParser::GetBlocksSize(); i++)
	{
		Server server;
		CreateServer((ConfigParser::GetBlock(i)), server);
		_servers.push_back(server);
	}
    if (_servers.size() > 1)
		CheckServers();
}

void ServersData::CreateServer(std::string block, Server& server)
{










    
}












void ServersData::CheckServers(void)
{

}