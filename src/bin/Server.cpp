#include "Server.hpp"

// Location
location& location::operator=(const location& rhs)
{
	if (this == &rhs)
		return (*this);
	autoIndex = rhs.autoIndex;
	uploadDir = rhs.uploadDir;
	root = rhs.root;
	allowMethods = rhs.allowMethods;
	index = rhs.index;
	redirect = rhs.redirect;
    return (*this);
}

// Server
Server::Server(void) { }

Server::Server(const Server& rhs) {
	this->_isDefaultServer = rhs._isDefaultServer;
	this->_serverName = rhs._serverName;
	this->_clientMaxBodySize = rhs._clientMaxBodySize;
	this->_root = rhs._root;
	this->_errorPage = rhs._errorPage;
	this->_port = rhs._port;
	this->_locations = rhs._locations;
	this->_cgi = rhs._cgi;
}

Server&	Server::operator=(const Server& rhs) {
	if (this == &rhs)
		return (*this);
	this->_isDefaultServer = rhs._isDefaultServer;
	this->_serverName = rhs._serverName;
	this->_clientMaxBodySize = rhs._clientMaxBodySize;
	this->_root = rhs._root;
	this->_errorPage = rhs._errorPage;
	this->_port = rhs._port;
	this->_locations = rhs._locations;
	this->_cgi = rhs._cgi;
	return (*this);
}

Server::~Server(void) { }

// Functions

// Getters
bool	Server::isDefaultServer(void)	const {
	return this->_isDefaultServer;
}

const std::string&	Server::getServerName(void)	const {
	return this->_serverName;
}

const std::string&	Server::getClientMaxBodySize(void)	const {
	return this->_clientMaxBodySize;
}

const std::string&	Server::getRoot(void)	const {
	return this->_root;
}

const std::map<short, std::string>&	Server::getErrorPage(void)	const {
	return this->_errorPage;
}

unsigned short int	Server::getPort(void)	const {
	return this->_port;
}

const std::vector<location>&	Server::getLocations(void)	const {
	return this->_locations;
}

const std::vector<std::string>&	Server::getCgi(void)	const {
	return this->_cgi;
}

// Setters
void	Server::setServerName(const std::string& serverName) {
	this->_serverName = serverName;
}
void	Server::setClientMaxBodySize(const std::string& clientMaxBodySize) {
	this->_clientMaxBodySize = clientMaxBodySize;
}

void	Server::setRoot(const std::string& root) {
	this->_root = root;
}
void	Server::setErrorPage(const std::map<short, std::string>& errorPage) {
	this->_errorPage = errorPage;
}
void	Server::setPort(unsigned short int port) {
	this->_port = port;
}
void	Server::setLocations(const std::vector<location>& location) {
	this->_locations = location;
}
void	Server::setCgi(const std::vector<std::string>& cgi) {
	this->_cgi = cgi;
}