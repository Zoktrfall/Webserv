#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <string>

enum HttpMethod
{
    GET,
    POST,
    DELETE,
    PUT,
    HEAD,
    NONE
};

class Request
{
    public :
        Request();
        void SetMethod(HttpMethod method);
        void SetPath(std::string& path);
        void SetVersion(std::string& version);

        HttpMethod GetMethod(void) const;
        const std::string& GetPath(void) const;
        const std::string& GetVersion(void) const;
    private :
        HttpMethod _method;
        std::string _path;
        std::string	_version;
        int	_port;
        std::unordered_map<std::string, std::string> _headers;
        std::unordered_map<std::string, std::string> _env_for_cgi;
        std::string	_body;
};

#endif