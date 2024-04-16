#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include "Server.hpp"
#include "IServersData.hpp"
#include "ConfigParser.hpp"

class ServersData : public IServersData {
	public:
		// ServersData(const std::string&);
		ServersData(char*);
		ServersData(const ServersData&);
		~ServersData(void);

		ServersData& operator=(const ServersData&);

		const std::vector<Server>&	getServers(void)	const;
		void						setServers(const std::vector<Server>&);
		inline bool					isOkay(void)		const;
		
	private:
		bool					_isOkay;
		std::vector<Server>		_servers;
		
		ServersData(void);
};
