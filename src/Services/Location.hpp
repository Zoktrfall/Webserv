#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <string>
#include <vector>
#include "HttpTypes.hpp"
#include "HttpController.hpp"

class Location
{
    public :
        Location(void);

		const std::string GetPath(void) const;
   		const std::string GetRoot(void) const; 
    	int GetAutoindex(void) const;
    	const std::vector<std::string>& GetIndices(void) const;
    	std::map<int, std::string> GetReturn(void) const;
    	std::string GetAlias(void) const;
    	long long GetClientMaxBodySize(void) const;
    	std::vector<std::string> GetCgiPath(void) const;
    	std::vector<std::string> GetCgiExt(void) const;
		std::vector<HttpMethod> GetLimitExcept(void) const;

		void SetPath(std::string& path);
    	void SetRoot(std::string& root);
    	void SetAutoindex(std::string& autoIndex);
    	void SetIndices(std::string& indices);
    	void SetReturn(std::string& ret);
    	void SetAlias(std::string& alias);
    	void SetClientMaxBodySize(std::string& client_max_body_size);
    	void SetCgiPath(std::string& cgiPath);
    	void SetCgiExt(std::string& cgiExt);
		void SetLimitExcept(std::string& limits);

    private :
		std::string _path;
		std::string _root;
		int _autoindex;
		long long _client_max_body_size;
		std::vector<std::string> _indices;
		std::map<int, std::string> _return;
		std::vector<HttpMethod> _limit_except;
		std::string	_alias;
		std::vector<std::string> _cgi_path;
		std::vector<std::string> _cgi_ext;
};

#endif