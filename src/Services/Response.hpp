#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <string>
#include <map>
#include <iostream>
#include "HttpTypes.hpp"

class Response
{
    public :
        Response(void);

        short GetStatusCode(void) const;
        std::string GetReasonPhrase(void) const;
        std::map<std::string, std::string> GetHeaders(void) const;
        std::string GetResponseBody(void) const;

        void SetStatusCode(short statusCode);
        void SetReasonPhrase(const std::string& reasonPhrase);
        void SetHeader(const std::string& headerName, const std::string& headerValue);
        void SetResponseBody(const std::string& responseBody);
        void Clear(void);

    private :
        short _statusCode;
        std::string _reasonPhrase;
        std::map<std::string, std::string> _headers;
        std::string _responseBody;
};



#endif