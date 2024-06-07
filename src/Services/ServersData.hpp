#ifndef SERVERS_DATA_HPP
#define SERVERS_DATA_HPP
#include <vector>
#include <string>
#include "Server.hpp"
#include "IServersData.hpp"
#include "ConfigParser.hpp"

class ServersData : public IServersData {
	public:
		// ServersData(const std::string&);
		ServersData(char*);
		ServersData(const ServersData&);
		~ServersData();

		ServersData& operator=(const ServersData&);

		const std::vector<Server>&	getServers(void)	const;
		void						setServers(const std::vector<Server>&);
		inline bool					isOkay(void)		const;
		
	private:
		bool					_isOkay;
		std::vector<Server>		_servers;
		
		ServersData(void);
};

#endif