#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include <map>
#include <cctype>
#include <algorithm>
#include "Tools.hpp"
#include "Location.hpp"

class Server
{
	public :
		Server(void);

		void AddLocation(void);
		void ClearDuplicates(void);
		void SetupErrorPages(void);

		const std::vector<uint16_t>& GetPorts(void) const;
		const std::vector<in_addr_t>& GetHosts(void) const;
		const std::string& GetRoot(void) const;
		const std::vector<std::string>& GetServerNames(void) const;
		long long GetClientMaxBodySize(void) const;
		int GetAutoIndex(void) const;
		const std::vector<std::string>& GetIndices(void) const;
		std::string GetIndex(size_t i) const;
		std::map<int, std::string> GetReturn(void) const;
		Location& GetLocation(size_t i);
		std::vector<Location> GetLocations(void) const; 
		std::string GetErrorPage(int ErrorCode);

		void SetPort(std::string& portStr);
		void SetRoot(std::string& root);
		void SetServerNames(std::string& serverName);
		void SetClientMaxBodySize(std::string client_max_body_size);
		void SetAutoIndex(std::string autoIndex);
		void SetIndices(std::string indices);
		void SetErrorPages(std::string& errorPage);
		void SetReturn(std::string& ret);

	private :
		std::vector<uint16_t> _ports;
		std::vector<in_addr_t> _hosts;
		std::vector<std::string> _server_names;
		std::string	_root;
		long long _client_max_body_size;
		int _autoindex;
		std::map<int, std::string> _return;
		std::vector<std::string> _indices;
		std::map<int, std::string> _error_pages;
		std::vector<Location> _locations;

		void InitErrorPages(void);
		void IsValidIPv4(const std::string& host);
		in_addr_t ConvertStringToInAddrT(std::string ipAddress);
		void RemoveDuplicates(std::vector<std::string>& vec);
		void RemoveDuplicates(std::vector<Location>& vec);
};

#endif