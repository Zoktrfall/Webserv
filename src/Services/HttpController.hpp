#ifndef HTTP_CONTROLLER_HPP
#define HTTP_CONTROLLER_HPP
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "IHttpController.hpp"
#include "Request.hpp"
#include "Tools.hpp"

#define LimitHeaders 4096
#define MessageBuffer 1048576
#define ConnectionTemeOut 60

class HttpController : public IHttpController
{
    public :
        RequestResult HttpRequest(int readSocket);
        void HttpResponse(int writeSocket);

        virtual ~HttpController() {};
        
    private :
        std::map<int, Request> _requests;

        bool CheckRequestIn(int sokcetId);
        void CreateNewRequest(int socketId);
        RequestResult ProcessHTTPRequest(int socketId);
        void ParseRequestHeaders(Request& request);
        void FirstRequestLine(const std::string& line, Request& request);
        void ParseHeaderLine(const std::string& line, Request& request);
        std::string ExtractBoundary(const std::string& contentType);
        RequestResult ParseBody(Request& request);
        RequestResult ParseChunked(Request& request);
};

#endif