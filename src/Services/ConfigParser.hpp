#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "FileOperation.hpp"
#include "Server.hpp"
#include "IConfigParser.hpp"

#define DEFAULT_PATH "./default.conf"

class ConfigParser : public IConfigParser {
	public:
		std::vector<Server> 		parse(const std::string);
		std::vector<std::string>	getContent();

};
