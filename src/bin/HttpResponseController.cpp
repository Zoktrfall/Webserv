#include "HttpResponseController.hpp"

void HttpResponseController::InitializeCodes(void)
{
    _codes[200] = "OK";
    _codes[201] = "Created";
    _codes[204] = "No Content";
    _codes[301] = "Moved Permanently";
    _codes[302] = "Found";
    _codes[400] = "Bad Request";
    _codes[401] = "Unauthorized";
    _codes[402] = "Payment Required";
    _codes[403] = "Forbidden";
    _codes[404] = "Not Found"; 
    _codes[405] = "Method Not Allowed";
    _codes[406] = "Not Acceptable";
    _codes[408] = "Request Timeout";
    _codes[411] = "Length Required";
    _codes[413] = "Payload Too Large";
    _codes[414] = "URI Too Long";
    _codes[415] = "Unsupported Media Type";
    _codes[429] = "Too Many Requests"; 
    _codes[431] = "Request Header Fields Too Large";
    _codes[500] = "Internal Server Error"; 
    _codes[501] = "Not Implemented";
    _codes[502] = "Bad Gateway";
    _codes[503] = "Service Unavailable";
}

bool HttpResponseController::CheckHeaderFields(std::map<std::string, std::string> headers)
{
    size_t headersLimit = 0;
    for(std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        if(it->second.size() > HeaderLineMaxSize)
            return true;
        headersLimit += it->second.size();
    }
    
    return headersLimit > LimitHeaders ? true : false;
}

std::string HttpResponseController::ReadFileAndCreateBody(std::string fileName)
{
    std::ifstream file(fileName);
    if(!file.is_open())
    {
        Logger::LogMsg(WARNING, "Failed to open File", _socketId);
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void HttpResponseController::ErrorCodeHandle(int code, Server& Server, Request& Request)
{
    std::string body = ReadFileAndCreateBody(Server.GetErrorPage(code));

    _response.SetStatusCode(code);
    _response.SetReasonPhrase(_codes[code]);
    _response.SetHeader("Server", "Miracle");
    _response.SetHeader("Date", Tools::GetCurrentDateTime());
    _response.SetHeader("Connection", "close");
    _response.SetHeader("Content-Type", "text/html");
    _response.SetHeader("Content-Language", "en-US");
    _response.SetHeader("Content-Length", Tools::ToString(body.size()));
    _response.SetResponseBody(body);
    if(Request.HasHeader("connection"))
        _response.SetHeader("Connection", Request.GetHeader("connection"));
    else
        _response.SetHeader("Connection", "close");
    if(Request.HasHeader("cookie"))
        _response.SetHeader("Set-Cookie", Request.GetHeader("cookie") + "; Path=/; Max-Age=3600;");
}

void HttpResponseController::CodeHandleWithBodyAndRequest(int code, std::string body, Request& Request)
{
    _response.SetStatusCode(code);
    _response.SetReasonPhrase(_codes[code]);
    _response.SetHeader("Server", "Miracle");
    _response.SetHeader("Date", Tools::GetCurrentDateTime());
    _response.SetHeader("Connection", "close");
    _response.SetHeader("Content-Type", "text/html");
    _response.SetHeader("Content-Language", "en-US");
    _response.SetHeader("Content-Length", Tools::ToString(body.size()));
    if(Request.GetMethod() != HEAD)
        _response.SetResponseBody(body);
    if(Request.HasHeader("connection"))
        _response.SetHeader("Connection", Request.GetHeader("connection"));
    else
        _response.SetHeader("Connection", "close");
    if(Request.HasHeader("cookie"))
        _response.SetHeader("Set-Cookie", Request.GetHeader("cookie") + "; Path=/; Max-Age=3600;");
}

bool HttpResponseController::CheckLimits(std::vector<HttpMethod> vec, Server& Server, Request& Request)
{
    std::string allowMethods = "";
    if(vec.empty())
        return false;

    for(size_t i = 0; i < vec.size(); i++)
    {
        if(Request.GetMethod() == vec[i])
            return false;
        if(vec[i] == GET)
            allowMethods += "GET ";
        else if(vec[i] == POST)
            allowMethods += "POST ";
        else if(vec[i] == HEAD)
            allowMethods += "HEAD ";
        else if(vec[i] == DELETE)
            allowMethods += "DELETE ";
    }

    _response.SetHeader("Allow", Tools::Trim(allowMethods, " "));
    ErrorCodeHandle(405, Server, Request);
    return true;
}

void HttpResponseController::CreateResponseAndSend(void)
{
    std::string httpResponse = "";
    std::map<std::string, std::string> headers = _response.GetHeaders();
    
    httpResponse += "HTTP/1.1 " + Tools::ToString(_response.GetStatusCode()) + " " + _response.GetReasonPhrase() + "\r\n";
    for(std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
        if(it->second != "")
            httpResponse += it->first + ": " + it->second + "\r\n";
    httpResponse += "\r\n" + _response.GetResponseBody();

    if(send(_socketId, httpResponse.c_str(), httpResponse.size(), 0) == -1)
        Logger::LogMsg(WARNING, strerror(errno));
    else
        Logger::LogMsg(DEBUG, "Response Sent To Socket", _socketId);

    _response.Clear();
}

bool HttpResponseController::CheckReturn(std::map<int, std::string> ret, Server& Server, Request& Request)
{
    if(ret.size() != 0)
    {
        std::map<int, std::string>::iterator it = ret.begin();
        if(it->second.empty())
            ErrorCodeHandle(it->first, Server, Request);
        else
            CodeHandleWithBodyAndRequest(it->first, it->second + "\r\n", Request);
        return true;
    }
    return false;
}

int HttpResponseController::FindRightLocation(std::string path, std::vector<Location> locations, std::string root)
{
    for(size_t i = 0; i < locations.size(); ++i)
    {
        std::string loc = locations[i].GetPath();
        if((root + Tools::Trim(path, "./")).find(root + Tools::Trim(loc, "./")) != std::string::npos)
            return i;
    }

    for(size_t i = 0; i < locations.size(); ++i)
    {
        std::string loc = locations[i].GetPath();
        if(Tools::Trim(root, "./") == Tools::Trim(loc, "./"))
            return i;
    }

    return -1;
}

std::string HttpResponseController::AliasPathCreate(std::string path, std::string alias)
{
    std::string fileName = path.substr(path.find_last_of("/") + 1);
    path = "." + alias + fileName;

    struct stat statbuf;
    if(stat(path.c_str(), &statbuf) == -1 || S_ISDIR(statbuf.st_mode))
        return "." + alias;
    
    return path;
}

bool HttpResponseController::CheckAndExecuteCgiScript(Server& Server, std::string path, Request& Request, int mainDirectoryIndex)
{
    std::string script = path.substr(path.find_last_of("/") + 1);
    std::vector<std::string> ext = Server.GetLocation(mainDirectoryIndex).GetCgiExt();
    std::vector<std::string> cgiPath = Server.GetLocation(mainDirectoryIndex).GetCgiPath();
    int index = -1;
    for(size_t i = 0; i < ext.size(); i++)
        if(Tools::EndsWith(path, ext[i]))
            index = i;
    if(index == -1)
        return false;


    int pipeIn[2], pipeOut[2];
    if(pipe(pipeIn) == -1 || pipe(pipeOut) == -1)
    {
        Logger::LogMsg(WARNING, strerror(errno), _socketId);
        ErrorCodeHandle(500, Server, Request);
        return true;
    }

    pid_t pid = fork();
    if(pid == -1)
    {
        Logger::LogMsg(WARNING, strerror(errno), _socketId);
        ErrorCodeHandle(500, Server, Request);
        return true;
    }

    if(pid == 0)
    {
        dup2(pipeIn[0], STDIN_FILENO);
        dup2(pipeOut[1], STDOUT_FILENO);
        close(pipeIn[1]);
        close(pipeOut[0]);

        std::string cgi_path_str = cgiPath[index];
        std::string request_path_str = path;

        char *cgi_path = const_cast<char*>(cgi_path_str.c_str());
        char *request_path = const_cast<char*>(request_path_str.c_str());

        char *const args[] = {cgi_path, request_path, NULL};
        char *const env[] = {NULL};

        execve(cgi_path, args, env);
        exit(1);
    } 
    else
    {
        close(pipeIn[0]);
        close(pipeOut[1]);

        int status;
        waitpid(pid, &status, 0);

        if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            ErrorCodeHandle(501, Server, Request);
            Logger::LogMsg(WARNING, "CGI script execution failed", _socketId);
            return true;
        }

        std::ostringstream oss;
        char buffer[1024];
        ssize_t bytesRead;
        while((bytesRead = read(pipeOut[0], buffer, sizeof(buffer))) > 0)
            oss.write(buffer, bytesRead);
        close(pipeOut[0]);

        CodeHandleWithBodyAndRequest(200, oss.str(), Request);
    }
    return true;
}

void HttpResponseController::ProcessGetHeadMethods(Server& Server, Request& Request)
{
    if(CheckReturn(Server.GetReturn(), Server, Request))
        return;

    std::string body = "";
    int mainDirectoryIndex = FindRightLocation(Request.GetPath(), Server.GetLocations(), Server.GetRoot());
    if(mainDirectoryIndex != -1)
    {
        if(CheckReturn(Server.GetLocation(mainDirectoryIndex).GetReturn(), Server, Request) )
            return;
        if(CheckLimits(Server.GetLocation(mainDirectoryIndex).GetLimitExcept(), Server, Request))
            return; 

        ThereIs what;
        std::string path = "";
        if(!Server.GetLocation(mainDirectoryIndex).GetRoot().empty())
            path = Server.GetLocation(mainDirectoryIndex).GetRoot() + Request.GetPath();
        else if(!Server.GetLocation(mainDirectoryIndex).GetAlias().empty())
            path = AliasPathCreate(Request.GetPath(), Server.GetLocation(mainDirectoryIndex).GetAlias());
        else
            path = Server.GetRoot() + Request.GetPath();

        if(CheckAndExecuteCgiScript(Server, path, Request, mainDirectoryIndex))
            return;

        what = Tools::PathExists(path);
        if(what == NotFound)
            ErrorCodeHandle(404, Server, Request);
        else if(what == File)
        {
            body = ReadFileAndCreateBody(path);
            CodeHandleWithBodyAndRequest(200, body, Request);
        }
        else if(what == Directory)
        {
            for(size_t i = 0; i < Server.GetLocation(mainDirectoryIndex).GetIndices().size(); i++)
            {
                body = ReadFileAndCreateBody(path + Server.GetIndex(i));
                if(!body.empty())
                    break;
            }
            if(body.empty())
            {
                if(Server.GetLocation(mainDirectoryIndex).GetAutoindex() == 1)
                {
                    body = Tools::GenerateHtmlFromDirectory(path);
                    CodeHandleWithBodyAndRequest(200, body, Request);
                }
                else
                    ErrorCodeHandle(404, Server, Request);
            }
            else
                CodeHandleWithBodyAndRequest(200, body, Request);
        }
        else if(what == Error)
            ErrorCodeHandle(500, Server, Request);
    }
    else
    {
        ThereIs what = Tools::PathExists(Server.GetRoot() + Request.GetPath());
        if(what == NotFound)
            ErrorCodeHandle(404, Server, Request);
        else if(what == File)
        {
            body = ReadFileAndCreateBody(Server.GetRoot() + Request.GetPath());
            CodeHandleWithBodyAndRequest(200, body, Request);
        }
        else if(what == Directory)
        {
            for(size_t i = 0; i < Server.GetIndices().size(); i++)
            {
                body = ReadFileAndCreateBody(Server.GetRoot() + Request.GetPath() + Server.GetIndex(i));
                if(!body.empty())
                    break;
            }
            if(body.empty())
            {
                if(Server.GetAutoIndex() == 1)
                {
                    body = Tools::GenerateHtmlFromDirectory(Server.GetRoot() + Request.GetPath());
                    CodeHandleWithBodyAndRequest(200, body, Request);
                }
                else
                    ErrorCodeHandle(404, Server, Request);
            }
            else
                CodeHandleWithBodyAndRequest(200, body, Request);
        }
        else if(what == Error)
            ErrorCodeHandle(500, Server, Request);
    }
}

bool HttpResponseController::DeleteFile(const std::string &path)
{
    if(path.find("/errors/") != std::string::npos)
        return true;

    if(std::remove(path.c_str()) == -1) 
    {
        Logger::LogMsg(WARNING, "Unable to delete file", _socketId);
        return true;
    }
    return false;
}

void HttpResponseController::ProcessDeleteMethod(Server& Server, Request& Request)
{
    if(CheckReturn(Server.GetReturn(), Server, Request))
        return;

    std::string body = "";
    int mainDirectoryIndex = FindRightLocation(Request.GetPath(), Server.GetLocations(), Server.GetRoot());
    if(mainDirectoryIndex != -1)
    {
        if(CheckReturn(Server.GetLocation(mainDirectoryIndex).GetReturn(), Server, Request) )
            return;
        if(CheckLimits(Server.GetLocation(mainDirectoryIndex).GetLimitExcept(), Server, Request))
            return; 

        ThereIs what;
        std::string path = "";
        if(!Server.GetLocation(mainDirectoryIndex).GetRoot().empty())
            path = Server.GetLocation(mainDirectoryIndex).GetRoot() + Request.GetPath();
        else if(!Server.GetLocation(mainDirectoryIndex).GetAlias().empty())
            path = AliasPathCreate(Request.GetPath(), Server.GetLocation(mainDirectoryIndex).GetAlias());
        else
            path = Server.GetRoot() + Request.GetPath();

        what = Tools::PathExists(path);
        if(what == NotFound)
            ErrorCodeHandle(404, Server, Request);
        else if(what == File)
        {
            if(DeleteFile(path))
                ErrorCodeHandle(403, Server, Request);
            else
                CodeHandleWithBodyAndRequest(204, body, Request);
        }
        else if(what == Directory)
            ErrorCodeHandle(403, Server, Request);
        else if(what == Error)
            ErrorCodeHandle(500, Server, Request);
    }
    else
    {
        ThereIs what = Tools::PathExists(Server.GetRoot() + Request.GetPath());
        if(what == NotFound)
            ErrorCodeHandle(404, Server, Request);
        else if(what == File)
        {
            if(DeleteFile(Server.GetRoot() + Request.GetPath()))
                ErrorCodeHandle(403, Server, Request);
            else
                CodeHandleWithBodyAndRequest(204, body, Request);
        }
        else if(what == Directory)
            ErrorCodeHandle(403, Server, Request);
        else if(what == Error)
            ErrorCodeHandle(500, Server, Request);
    }
}



std::string HttpResponseController::ExtractFilename(const std::string& contentDisposition)
{
    std::string filename = "";
    std::string key = "filename=";
    size_t startPos = contentDisposition.find(key);

    if(startPos != std::string::npos)
    {
        startPos += key.length();
        if(contentDisposition[startPos] == '"')
        {
            startPos++;
            size_t endPos = contentDisposition.find('"', startPos);
            if(endPos != std::string::npos)
                filename = contentDisposition.substr(startPos, endPos - startPos);
        } 
        else 
        {
            size_t endPos = contentDisposition.find(';', startPos);
            if(endPos == std::string::npos)
                endPos = contentDisposition.length();
            filename = contentDisposition.substr(startPos, endPos - startPos);
        }
    }

    return filename;
}

void HttpResponseController::ProcessPostMethod(Server& Server, Request& Request)
{
    if(CheckReturn(Server.GetReturn(), Server, Request))
        return;
    if(Request.GetBody().size() >= static_cast<size_t>(Server.GetClientMaxBodySize()))
    {
        ErrorCodeHandle(413, Server, Request);
        return;
    }
    
    std::string body = "";
    int mainDirectoryIndex = FindRightLocation(Request.GetPath(), Server.GetLocations(), Server.GetRoot());
    if(mainDirectoryIndex != -1)
    {
        if(CheckReturn(Server.GetLocation(mainDirectoryIndex).GetReturn(), Server, Request) )
            return;
        if(CheckLimits(Server.GetLocation(mainDirectoryIndex).GetLimitExcept(), Server, Request))
            return; 
        if(Request.GetBody().size() >= static_cast<size_t>(Server.GetLocation(mainDirectoryIndex).GetClientMaxBodySize()))
        {
            ErrorCodeHandle(413, Server, Request);
            return;
        }

        ThereIs what;
        std::string path = "";
        if(!Server.GetLocation(mainDirectoryIndex).GetRoot().empty())
            path = Server.GetLocation(mainDirectoryIndex).GetRoot() + Request.GetPath();
        else if(!Server.GetLocation(mainDirectoryIndex).GetAlias().empty())
            path = AliasPathCreate(Request.GetPath(), Server.GetLocation(mainDirectoryIndex).GetAlias());
        else
            path = Server.GetRoot() + Request.GetPath();

        if(CheckAndExecuteCgiScript(Server, path, Request, mainDirectoryIndex))
            return;

        what = Tools::PathExists(path);
        if(what == NotFound)
            ErrorCodeHandle(400, Server, Request);
        else if(what == File)
            ErrorCodeHandle(403, Server, Request);
        else if(what == Directory)
        {
            if(!Request.HasHeader("content-type"))
            {
                ErrorCodeHandle(400, Server, Request);
                return;
            }

            std::string fileName = ExtractFilename(Request.GetHeader("content-disposition"));
            if(fileName.empty())
            {
                ErrorCodeHandle(405, Server, Request);
                return;
            }

            std::ofstream outFile(path + "/" + fileName);
            if(!outFile)
            {
                Logger::LogMsg(WARNING, "Error opening file", _socketId);
                ErrorCodeHandle(429, Server, Request);
                return;
            }
            outFile << Request.GetBody();
            outFile.close();
            CodeHandleWithBodyAndRequest(201, body, Request);
        }
        else if(what == Error)
            ErrorCodeHandle(500, Server, Request);
    }
    else
    {
        ThereIs what = Tools::PathExists(Server.GetRoot() + Request.GetPath());
        if(what == NotFound)
            ErrorCodeHandle(400, Server, Request);
        else if(what == File)
            ErrorCodeHandle(403, Server, Request);
        else if(what == Directory)
        {
            if(!Request.HasHeader("content-type"))
            {
                ErrorCodeHandle(400, Server, Request);
                return;
            }

            std::string fileName = ExtractFilename(Request.GetHeader("content-disposition"));
            if(fileName.empty())
            {
                ErrorCodeHandle(405, Server, Request);
                return;
            }

            std::ofstream outFile(Server.GetRoot() + Request.GetPath() + "/" + fileName);
            if(!outFile)
            {
                Logger::LogMsg(WARNING, "Error opening file", _socketId);
                ErrorCodeHandle(429, Server, Request);
                return;
            }
            outFile << Request.GetBody();
            outFile.close();
            CodeHandleWithBodyAndRequest(201, body, Request);
        }
        else if(what == Error)
            ErrorCodeHandle(500, Server, Request);
    }
}


void HttpResponseController::HttpResponse(int socketId, Server& Server, Request& Request)
{
    _socketId = socketId;
    InitializeCodes();

    if(Request.GetPath().size() >= URIMaxSize)
        ErrorCodeHandle(414, Server, Request);
    else if(CheckHeaderFields(Request.GetHeaders()))
        ErrorCodeHandle(431, Server, Request);
    else if(Request.GetMethod() == GET || Request.GetMethod() == HEAD)
        ProcessGetHeadMethods(Server, Request);
    else if(Request.GetMethod() == POST)
        ProcessPostMethod(Server, Request);
    else if(Request.GetMethod() == DELETE)
        ProcessDeleteMethod(Server, Request);
    else
        ErrorCodeHandle(400, Server, Request);

    CreateResponseAndSend();
}