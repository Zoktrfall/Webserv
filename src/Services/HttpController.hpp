#ifndef HTTP_CONTROLLER_HPP
#define HTTP_CONTROLLER_HPP
#include <string>
#include <sstream>
#include "IHttpController.hpp"
#include "Request.hpp"
#include "Tools.hpp"

#define RecvSize 4096
#define LimitRequestBody 262144
#define LimitChunk 262144
#define CGIBufSize 4096
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
        RequestResult ParseBody(Request& request);
        RequestResult ParseChunked(Request& request);
};

#endif