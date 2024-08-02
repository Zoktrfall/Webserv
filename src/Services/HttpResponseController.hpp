#ifndef HTTP_RESPONSE_CONTROLLER_HPP
#define HTTP_RESPONSE_CONTROLLER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdio>
#include <dirent.h>
#include <algorithm>
#include <fstream>
#include "Tools.hpp"
#include "IHttpResponseController.hpp"
#include "HttpRequestController.hpp"
#include "ServersData.hpp"
#include "HttpTypes.hpp"

#define URIMaxSize 50
#define HeaderLineMaxSize 300

class HttpResponseController : public IHttpResponseController
{
    public :
        void HttpResponse(int socketId, Server& Server, Request& Request);

    private :
        int _socketId;
        std::map<int, std::string> _codes;
        Response _response;

        void ProcessGetHeadMethods(Server& Server, Request& Request);
        void ProcessPostMethod(Server& Server, Request& Request);
        void ProcessDeleteMethod(Server& Server, Request& Request);


        bool CheckHeaderFields(std::map<std::string, std::string> headers);
        bool CheckAndExecuteCgiScript(Server& Server, std::string path, Request& Requet, int mainDirectoryIndex);

        int FindRightLocation(std::string path, std::vector<Location> locations, std::string root);

        void CreateResponseAndSend(void);
        std::string AliasPathCreate(std::string path, std::string alias);
        std::string ReadFileAndCreateBody(std::string fileName);
        bool DeleteFile(const std::string &path);
        std::string ExtractFilename(const std::string& contentDisposition);

        void InitializeCodes(void);
        void ErrorCodeHandle(int code, Server& Server, Request& Request);
        void CodeHandleWithBodyAndRequest(int code, std::string body, Request& Request);

        bool CheckReturn(std::map<int, std::string> ret, Server& Server, Request& Request);
        bool CheckLimits(std::vector<HttpMethod> vec, Server& Server, Request& Request);
};

#endif