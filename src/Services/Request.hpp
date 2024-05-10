#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <string>

#include <iostream>

enum HttpRequestStatus
{
    InProgress,
    Completed
};

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

        void Status(HttpRequestStatus status);
        void SetFirstLineCheck(bool firstLineCheck);
        void ReadFurther(bool requestComplete);
        void AreHeadersFinished(bool headersRead);
        void AppendRequestContent(std::string partRequestContent);
        void SetRequestContent(std::string& requestContent);

        void SetSocketId(const int socketId);
        void SetMethod(HttpMethod method);
        void SetPath(std::string& path);
        void SetVersion(std::string& version);
        void SetHeader(const std::string& headerName, const std::string& headerValue);
        void SetBody(const std::string& partBody);

        HttpRequestStatus Status(void) const;
        bool GetFirstLineCheck(void) const;
        bool ReadFurther(void) const;
        bool AreHeadersFinished(void) const;
        const std::string& GetRequestContent(void) const;

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
        HttpRequestStatus _HttpRequestStatus;
        bool _firstLineCheck;
        bool _requestComplete;
        bool _headersRead;
        std::string _requestContent;
        int _socketId;

        HttpMethod _method;
        std::string _path;
        std::string	_version;
        std::unordered_map<std::string, std::string> _headers;
        std::string	_body;
};

#endif