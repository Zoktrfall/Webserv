#ifndef SERVERS_DATA_HPP
#define SERVERS_DATA_HPP
#include "IServersData.hpp"
#include "Server.hpp"
#include "ConfigFile.hpp"
#include "ServerDataExc.hpp"
#include "ConfigParser.hpp"
#include "Tools.hpp"
#include "Logger.hpp"

class ServersData : public ConfigParser, IServersData
{
	public:
		ServersData(const char* configFilePath);
		bool SetupServersData(void);

		std::vector<Server>& GetServers(void);
		
	private:
		ConfigFile _configFile;
		std::string	_content;
		std::vector<Server>	_servers;

		void LoadServers(void);
		void CreateServer(std::string block, Server& Server);
		void CheckServers(Server& Server);
		void HasConflicts(void);
		bool HasIntersection(const std::vector<uint16_t>& vec1, const std::vector<uint16_t>& vec2);
};

#endif