#include "HttpController.hpp"

bool HttpController::CheckRequestIn(int socketId) { return _requests.count(socketId) > 0 ? true : false; }
void HttpController::CreateNewRequest(int socketId) { _requests[socketId] = Request(); }
void HttpController::FirstRequestLine(const std::string& line, Request& request)
{
    std::istringstream iss(line);
    std::string method, path, version;
    iss >> method >> path >> version;
    HttpMethod parsedMethod = NONE;
    if(method == "GET")
        parsedMethod = GET;
    else if(method == "POST")
        parsedMethod = POST;
    else if(method == "DELETE")
        parsedMethod = DELETE;
    else if(method == "PUT")
        parsedMethod = PUT;
    else if(method == "HEAD")
        parsedMethod = HEAD;

    request.SetMethod(parsedMethod);
    request.SetPath(path);
    request.SetVersion(version);
}
void HttpController::ParseHeaderLine(const std::string& line, Request& request)
{
    std::istringstream iss(line);
    std::string headerName, headerValue;

    std::getline(iss, headerName, ':');
    std::getline(iss, headerValue);

    if(!request.HasHeader(headerName))
        request.SetHeader(Tools::ToLower(headerName), Tools::Trim(headerValue, WhiteSpaces));
}
std::string HttpController::ExtractBoundary(const std::string& contentType)
{
    std::string boundaryKey = "boundary=";
    size_t boundaryPos = contentType.find(boundaryKey);
    if (boundaryPos != std::string::npos)
    {
        std::string key = contentType.substr(boundaryPos + boundaryKey.length());
        if(key != "")
            return "--" + key + "--";
    }
    return "";
}
void HttpController::ParseRequestHeaders(Request& request)
{
    size_t headersEndPos = request.GetRequestContent().find("\r\n\r\n");
    if(headersEndPos == std::string::npos || headersEndPos > LimitHeaders)
        return ;
    std::string headers = request.GetRequestContent().substr(0, headersEndPos);
    std::string newRequestContent = request.GetRequestContent().substr(headersEndPos + 4);
    std::istringstream iss(headers);
    std::string line;
    bool firstLineCheck = false;

    while(std::getline(iss, line))
    {   
        while(!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        if(!firstLineCheck)
        {
            FirstRequestLine(line, request);
            firstLineCheck = true;
        }
        else if(line.find(":") != std::string::npos)
            ParseHeaderLine(line, request);
    }

    request.SetRequestContent(newRequestContent);
    if(request.GetHeader("content-type").find("multipart/form-data") != std::string::npos)
        request.SetBoundaryKey(ExtractBoundary(request.GetHeader("content-type")));
}
RequestResult HttpController::ParseBody(Request& request)
{
    int contentLength = std::atoi(request.GetHeader("content-length").c_str());
    request.SetBody(request.GetRequestContent().substr(0, contentLength));

    if(request.GetBoundaryKey() != "" && request.GetBody().rfind(request.GetBoundaryKey()) == std::string::npos)
        return Multipart;

    return Success;
}
RequestResult HttpController::ParseChunked(Request& request)
{
    std::string requestContent = request.GetRequestContent();
    if(request.GetChunkSize() == -1)
    {
        std::string block = requestContent.substr(0, requestContent.find("\r\n"));
        request.SetChunkSize(static_cast<int>(std::strtol(block.c_str(), NULL, 16)));
    }

    // Why am I adding 2 and 4 because when I use .find("0\r\n\r\n") 
    // it removes the line that doesn't have \r\n in it after 
    // I need to skip \r\n the chunk, that's why I plus 2 and 4
    std::string chunkSizeStr = Tools::ToString(request.GetChunkSize());
    request.SetChunk(requestContent.substr(chunkSizeStr.length() + 2, request.GetChunkSize()));
    std::string newRequestContent = requestContent.substr(request.GetChunkSize() + chunkSizeStr.length() + 4);

    chunkSizeStr = newRequestContent.substr(0, newRequestContent.find("\r\n"));
    request.SetChunkSize(static_cast<int>(std::strtol(chunkSizeStr.c_str(), NULL, 16)));
    if(request.GetChunkSize() == 0)
        return Success;
    
    request.SetRequestContent(newRequestContent);
    return Chunked;
}
RequestResult HttpController::ProcessHTTPRequest(int socketId)
{
    RequestResult requestResult = Success;
    while(true)
    {
        char requestBuffer[MessageBuffer];
        int bytesRead = -1;

        requestResult = Tools::Recv(socketId, requestBuffer, bytesRead);
        if(requestResult != Success)
            return requestResult;

        if(bytesRead == -1)
            break;

        _requests[socketId].AppendRequestContent(requestBuffer, bytesRead);
    }
    ParseRequestHeaders(_requests[socketId]);

    if(_requests[socketId].HasHeader("content-length"))
        requestResult = ParseBody(_requests[socketId]);
    else if(_requests[socketId].HasHeader("transfer-encoding") &&
        !_requests[socketId].GetHeader("transfer-encoding").compare("chunked"))
        requestResult = ParseChunked(_requests[socketId]);

    _requests[socketId].ClearContent();
    return requestResult;
}
RequestResult HttpController::HttpRequest(int readSocket)
{
    if(!CheckRequestIn(readSocket))
        CreateNewRequest(readSocket);

    RequestResult requestResult = ProcessHTTPRequest(readSocket);
    if(requestResult == ReadError || requestResult == ClosedConnection)
        _requests.erase(readSocket);

    return requestResult;
}





std::string ExtractFilename(const std::string& contentDisposition) {
    std::string filename;
    std::string key = "filename=";
    size_t startPos = contentDisposition.find(key);

    if (startPos != std::string::npos) {
        startPos += key.length();
        if (contentDisposition[startPos] == '"') {
            startPos++;
            size_t endPos = contentDisposition.find('"', startPos);
            if (endPos != std::string::npos) {
                filename = contentDisposition.substr(startPos, endPos - startPos);
            }
        } else {
            size_t endPos = contentDisposition.find(';', startPos);
            if (endPos == std::string::npos) {
                endPos = contentDisposition.length();
            }
            filename = contentDisposition.substr(startPos, endPos - startPos);
        }
    }

    return filename;
}
void HttpController::HttpResponse(int readSocket) //* Needs some work *
{
    if(_requests[readSocket].GetRequestContent().length() > 100000000)
    {
        // std::cout<<"Code: 413"<<std::endl;
        const char *http_response = "HTTP/1.1 413 Content Too Large\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<title>Welcome to nginx!</title>\n<style>\nhtml { color-scheme: light dark; }\nbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n</style>\n</head>\n<body>\n<h1>Welcome to nginx!</h1>\n<p>If you see this page, the nginx web server is successfully installed and working. Further configuration is required.</p>\n<p>For online documentation and support please refer to <a href=\"http://nginx.org/\">nginx.org</a>.<br/>Commercial support is available at <a href=\"http://nginx.com/\">nginx.com</a>.</p>\n<p><em>Thank you for using nginx.</em></p>\n</body>\n</html>";
        std::cout<<"Send: "<<send(readSocket, http_response, strlen(http_response), 0)<<std::endl;
    }
    else
    {   

        std::string filename = ExtractFilename(_requests[readSocket].GetHeader("content-disposition"));
        std::cout<<"filename: "<<filename<<std::endl;
        std::ofstream outFile(filename);
        outFile << _requests[readSocket].GetBody();
        outFile.close(); // Закрываем файл



    std::cout << "Code: 200" << std::endl;
    const char* http_response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n\r\n"
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "<meta charset=\"UTF-8\">\n"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "<title>My Web Page</title>\n"
    "<style>\n"
    "body {\n"
    "font-family: Arial, sans-serif;\n"
    "margin: 0;\n"
    "padding: 0;\n"
    "box-sizing: border-box;\n"
    "display: flex;\n"
    "justify-content: center;\n"
    "align-items: center;\n"
    "height: 100vh;\n"
    "background-color: #333;\n"
    "color: #fff;\n"
    "}\n"
    ".container {\n"
    "text-align: center;\n"
    "}\n"
    "h1 {\n"
    "color: #fff;\n"
    "}\n"
    "p {\n"
    "color: #ccc;\n"
    "}\n"
    "#uploadForm {\n"
    "background-color: #666;\n"
    "padding: 20px;\n"
    "margin-top: 20px;\n"
    "}\n"
    "button {\n"
    "background-color: #ccc;\n"
    "color: #333;\n"
    "border: none;\n"
    "padding: 10px 20px;\n"
    "cursor: pointer;\n"
    "transition: background-color 0.3s ease;\n"
    "}\n"
    "button:hover {\n"
    "background-color: #fff;\n"
    "color: #333;\n"
    "}\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<div class=\"container\">\n"
    "<h1>This is a simple HTML file.</h1>\n"
    "<p>Feel free to modify and add your content here!</p>\n"
    "<form id=\"uploadForm\" enctype=\"multipart/form-data\">\n"
    "<input type=\"file\" name=\"file\" id=\"fileInput\" required />\n"
    "<button type=\"submit\">Upload File</button>\n"
    "</form>\n"
    "<div id=\"statusMessage\"></div>\n"
    "</div>\n"
    "<script type=\"text/javascript\">\n"
    "document.addEventListener('DOMContentLoaded', () => {\n"
    "const form = document.getElementById('uploadForm');\n"
    "const statusMessage = document.getElementById('statusMessage');\n"
    "form.addEventListener('submit', async (e) => {\n"
    "e.preventDefault();\n"
    "const formData = new FormData(form);\n"
    "try {\n"
    "const response = await fetch('/upload', {\n"
    "method: 'POST',\n"
    "body: formData,\n"
    "});\n"
    "if (!response.ok) {\n"
    "throw new Error('Upload failed');\n"
    "}\n"
    "statusMessage.textContent = 'File uploaded successfully!';\n"
    "form.reset();\n"
    "} catch (error) {\n"
    "statusMessage.textContent = 'Upload failed. Please try again.';\n"
    "console.error('Error:', error);\n"
    "}\n"
    "});\n"
    "});\n"
    "</script>\n"
    "</body>\n"
    "</html>";


    std::cout << "Send: " << send(readSocket, http_response, strlen(http_response), 0) << std::endl;
    }
    _requests.erase(readSocket);
}