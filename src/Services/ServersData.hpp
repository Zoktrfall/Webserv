#ifndef SERVERS_DATA_HPP
#define SERVERS_DATA_HPP
#include "Server.hpp"
#include "ConfigFile.hpp"
#include "ServerDataExc.hpp"
#include "ConfigParser.hpp"
#include "Tools.hpp"

class ServersData : public ConfigParser
{
	public:
		ServersData(const char* configFilePath);
		bool SetupServersData(void);
		
	private:
		ConfigFile _configFile;
		std::string	_content;
		// std::vector<Server>	 _servers;
};

#endif