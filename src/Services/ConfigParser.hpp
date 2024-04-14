#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "FileOperation.hpp"
#include "Server.hpp"
#include "IConfigParser.hpp"

class ConfigParser : public IConfigParser {
	public:
		std::vector<Server> parseConfigFile(const std::string&);

};
