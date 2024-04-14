#include "ServersData.hpp"
#include "ConfigParser.hpp"

ServersData::ServersData(void) { }

// ServersData::ServersData(const std::string& filePath) {
// 	ConfigParser	parser;
// 	(void) filePath;
// 	(void) parser;
// }

ServersData::ServersData(char* filePath) {
	if(filePath == NULL)
		exit(1);
}


ServersData::ServersData(const ServersData& rhs) {
	this->_isOkay = rhs._isOkay;
	this->_servers = rhs._servers;
}

ServersData& ServersData::operator=(const ServersData& rhs) {
	if (this == &rhs)
		return (*this);
	this->_isOkay = rhs._isOkay;
	this->_servers = rhs._servers;
	return (*this);
}

ServersData::~ServersData(void) {
	
}

// Functions
inline bool	ServersData::isOkay(void)	const {
	return (this->_isOkay);
}

// Getters
const std::vector<Server>& ServersData::getServers(void) const {
	return this->_servers;
}

// Setters
void	ServersData::setServers(const std::vector<Server>& servers) {
	this->_servers = servers;
}