#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include "Server.hpp"
#include "IServersData.hpp"
#include "ConfigParser.hpp"

class ServersData : public IServersData {
	public:
		ServersData(const char*);
		ServersData(const ServersData&);
		~ServersData(void);

		ServersData& operator=(const ServersData&);

		inline bool					isOkay(void)		const;
		const std::vector<Server>&	getServers(void)	const;
		void						setServers(const std::vector<Server>&);
		
	private:
		bool					_isOkay;
		std::vector<Server>		_servers;
		
		ServersData(void);
};
