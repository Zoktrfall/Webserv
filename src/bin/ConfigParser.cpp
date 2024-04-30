#include "ConfigParser.hpp"
#include "Server.hpp"
#include "FileOperation.hpp"

std::vector<Server> ConfigParser::parse(const std::string filePath) {
	std::vector<Server>	servers;
	std::string			configContent = FileOperation::readFile(filePath);

	std::cout << configContent << std::endl;

	return servers;
}