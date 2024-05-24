#ifndef HTTP_CONTROLLER_HPP
#define HTTP_CONTROLLER_HPP
#include "IHttpController.hpp"
#include "Request.hpp"
#include "Tools.hpp"

# define RECV_SIZE 4096
# define CGI_BUFSIZE 4096

class HttpController : public IHttpController
{
    public :
        bool HttpRequest(int readSocket);
        void HttpResponse(int writeSocket);

        virtual ~HttpController() {};
    private :
        std::map<int, Request> _requests;

        bool CheckRequestIn(int sokcetId);
        void CreateNewRequest(int socketId);
        bool ProcessHTTPRequest(int socketId);
        void ParseRequestHeaders(Request& request);
        void FirstRequestLine(const std::string& line, Request& request);
        void ParseHeaderLine(const std::string& line, Request& request);
        void ParseBody(Request& request);
        void ParseChunked(Request& request);
};

#endif