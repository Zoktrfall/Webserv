#pragma once

#include <vector>
#include "Server.hpp"

class IServersData
{
	public:
		virtual ~IServersData(void) {};

		virtual inline bool					isOkay(void)		const = 0;
		virtual const std::vector<Server>&	getServers(void)	const = 0;
		virtual void						setServers(const std::vector<Server>&) = 0;
};
