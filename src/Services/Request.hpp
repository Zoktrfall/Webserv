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
        void ReadFurther(bool requestComplete);

        void SetSocketId(const int socketId);
        void SetMethod(HttpMethod method);
        void SetPath(std::string& path);
        void SetVersion(std::string& version);
        void SetHeader(const std::string& headerName, const std::string& headerValue);
        void SetBody(const std::string& partBody);

        bool GetFirstLineCheck(void) const;
        bool ReadFurther(void) const;

        int GetSocketId(void) const;
        HttpMethod GetMethod(void) const;
        const std::string& GetPath(void) const;
        const std::string& GetVersion(void) const;
        const std::string& GetHeader(const std::string& headerName);
        bool HasHeader(const std::string& headerName) const;
        const std::string& GetBody(void) const;



        void printHeaders() const {
            for (const auto& pair : _headers) {
                std::cout << "Header: " << pair.first << " -> " << pair.second << std::endl;
            }
        }

    private :
        bool _firstLineCheck;
        bool _requestComplete;
        int _socketId;

        HttpMethod _method;
        std::string _path;
        std::string	_version;
        std::unordered_map<std::string, std::string> _headers;
        std::string	_body;
};

#endif