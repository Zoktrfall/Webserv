#pragma once

#include <iostream>
#include <string>
#include "FileOperation.hpp"
#include "Server.hpp"

class IConfigParser {
	public:
		virtual ~IConfigParser(void) {}

		virtual std::vector<Server> parseConfigFile(const std::string&) = 0;
};