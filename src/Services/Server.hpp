#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>

class Server //* Needs some work *
{
	public:
		Server(void);
		Server(const Server&);
		~Server();

		Server&	operator=(const Server&);
		
		bool								isDefaultServer(void)		const;
		const std::string&					getServerName(void)			const;
		const std::string&					getClientMaxBodySize(void)	const;
		const std::string&					getRoot(void)				const;
		const std::map<short, std::string>&	getErrorPage(void)			const;
		unsigned short int					getPort(void)				const;
		// const std::vector<location>&		getLocations(void)			const;
		const std::vector<std::string>&		getCgi(void)				const;

		void	setServerName(const std::string&);
		void	setClientMaxBodySize(const std::string&);
		void	setRoot(const std::string&);
		void	setErrorPage(const std::map<short, std::string>&);
		void	setPort(unsigned short int);
		// void	setLocations(const std::vector<location>&);
		void	setCgi(const std::vector<std::string>&);
	
	private:
		bool							_isDefaultServer;
		std::string						_serverName;
		std::string						_clientMaxBodySize;
		std::string						_root;
		std::map<short, std::string>	_errorPage;
		unsigned short int				_port;
		// std::vector<location>			_locations;
		std::vector<std::string>		_cgi;
};

#endif