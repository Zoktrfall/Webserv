#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <string>

#include <iostream>

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

        void SetFirstLineCheck(bool firstLineCheck);

        void SetMethod(HttpMethod method);
        void SetPath(std::string& path);
        void SetVersion(std::string& version);
        void SetHeader(const std::string& headerName, const std::string& headerValue);


        bool GetFirstLineCheck(void) const;

        HttpMethod GetMethod(void) const;
        const std::string& GetPath(void) const;
        const std::string& GetVersion(void) const;

        void printHeaders() const {
            for (const auto& pair : _headers) {
                std::cout << "Header: " << pair.first << " -> " << pair.second << std::endl;
            }
        }

    private :
        bool _firstLineCheck;

        HttpMethod _method;
        std::string _path;
        std::string	_version;
        std::unordered_map<std::string, std::string> _headers;
        std::string	_body;
};

#endif