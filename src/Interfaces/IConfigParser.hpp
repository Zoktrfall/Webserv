#pragma once

#include <iostream>
#include <string>
#include "FileOperation.hpp"
#include "Server.hpp"

class IConfigParser {
	public:
		virtual ~IConfigParser(void) {}

		virtual std::vector<Server> parse(const std::string) = 0;
};