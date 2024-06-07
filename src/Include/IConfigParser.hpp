#ifndef I_CONFIG_PARSER_HPP
#define I_CONFIG_PARSER_HPP
#include <iostream>
#include <string>
#include "FileOperation.hpp"
#include "Server.hpp"

class IConfigParser {
	public:
		virtual ~IConfigParser() {}

		virtual std::vector<Server> parseConfigFile(const std::string&) = 0;
};

#endif