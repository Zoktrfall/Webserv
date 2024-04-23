#include "ConfigParser.hpp"
#include "Server.hpp"

std::vector<Server> ConfigParser::parse(const std::string filePath) {
	std::vector<Server>	servers;
	(void)filePath;
	std::cout << filePath << std::endl;
	Server s;
	s.setServerName(std::string("Arman"));
	servers.push_back(s);
	return servers;
}