#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <string>
#include <iostream>
#include "HttpTypes.hpp"

class Request
{
    public :
        Request(void);

        void ReadAllDataFromSocket(bool socketRead);
        void AppendRequestContent(std::string partRequestContent);
        void SetRequestContent(std::string requestContent);
        void SetBoundaryKey(std::string boundaryKey);

        void SetMethod(HttpMethod method);
        void SetPath(std::string& path);
        void SetVersion(std::string& version);
        void SetHeader(const std::string& headerName, const std::string& headerValue);
        void SetBody(const std::string& partBody);
        void SetChunk(const std::string& chunk);
        void SetChunkSize(const int chunkSize);

        bool ReadAllDataFromSocket(void) const;
        const std::string& GetRequestContent(void) const;
        const std::string& GetBoundaryKey(void) const;

        HttpMethod GetMethod(void) const;
        const std::string& GetPath(void) const;
        const std::string& GetVersion(void) const;
        const std::string& GetHeader(const std::string& headerName);
        bool HasHeader(const std::string& headerName) const;
        const std::string& GetBody(void) const;
        const std::string& GetChunk(void) const;
        int GetChunkSize(void) const;

        void printMap() {
            for (auto iter = _headers.begin(); iter != _headers.end(); ++iter) {
                std::cout << iter->first << ": " << iter->second << std::endl;
            }
        }
        
        std::string _requestContent;
    private :
        bool _socketRead;
        std::string _boundaryKey;

        HttpMethod _method;
        std::string _path;
        std::string	_version;
        std::map<std::string, std::string> _headers;
        std::string	_body;
        std::string _chunks;
        int _chunkSize;
};

#endif