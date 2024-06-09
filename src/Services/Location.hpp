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

    private :
		std::string _path;
		std::string _root;
		bool _autoindex;
		std::string	_index;
		std::vector<HttpMethod> _methods;
		std::string _return;
		std::string	_alias;
		unsigned long _client_max_body_size;
		std::vector<std::string> _cgi_path;
		std::vector<std::string> _cgi_ext;
};

#endif