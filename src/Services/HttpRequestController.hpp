#ifndef HTTP_REQUEST_CONTROLLER_HPP
#define HTTP_REQUEST_CONTROLLER_HPP
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "IHttpRequestController.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Tools.hpp"

#define LimitHeaders 4096
#define MessageBuffer (1024 * 1024)
#define ConnectionTemeOut 60

class HttpRequestController : public IHttpRequestController
{
    public :
        RequestResult HttpRequest(int readSocket);
        virtual ~HttpRequestController() {};

        Request& GetRequest(int index);
        void ClearRequest(int index);

    private :
        std::map<int, Request> _requests;
        
        bool CheckRequestIn(int sokcetId);
        void CreateNewRequest(int socketId);
        RequestResult ProcessHTTPRequest(int socketId);
        void ParseRequestHeaders(Request& request);
        void FirstRequestLine(const std::string& line, Request& request);
        void ParseHeaderLine(const std::string& line, Request& request);
        void CheckMultiPart(Request& request, int socketId);
        std::string ExtractBoundary(const std::string& contentType);
        RequestResult ParseBody(Request& request);
        RequestResult ParseChunked(Request& request);
};

#endif