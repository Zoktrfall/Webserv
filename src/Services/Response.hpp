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

    private :
        std::string _response;

        short _statusCode;
        std::string _reasonPhrase;
        std::map<std::string, std::string> _headers;

        // std::string _serverName; //Server: nginx/1.27.0
        // std::string _date; //Date: Tue, 15 Nov 1994 08:12:31 GMT
        // size_t _contentLength; //Content-Length: 1234
        // std::string _contetType; //Content-Type: text/html
        // std::string _contentEncoding; //Content-Encoding: gzip
        // std::string _contentLanguage; //Content-Language: en
        // std::string _connection; //Connection: close
        // std::string _cacheControl; //Cache-Control: no-cache, no-store, must-revalidate
        // std::string _setCookie; //Set-Cookie: sessionId=abcd1234; Path=/; HttpOnly
        // std::string _allow; //Allow: GET, HEAD
        // std::string _contentLocation; //Content-Location: /index.html
        // std::string _location; //Location: http://www.example.com/newpage.html
        // std::string _retryAfter; //Retry-After: 120
};



#endif