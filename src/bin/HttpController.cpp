#include "HttpController.hpp"
#include <unistd.h>
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

std::string extractBoundary(const std::string& contentType)
{
    std::string boundaryKey = "boundary=";
    size_t boundaryPos = contentType.find(boundaryKey);
    if (boundaryPos != std::string::npos)
        return "--" + contentType.substr(boundaryPos + boundaryKey.length());
    return "";
}
void HttpController::ParseRequestHeaders(Request& request)
{
    std::string headers = request.GetRequestContent().substr(0, request.GetRequestContent().find("\r\n\r\n"));
    std::string newRequestContent = request.GetRequestContent().substr(request.GetRequestContent().find("\r\n\r\n") + 4);
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
        else
            ParseHeaderLine(line, request);
    }

    request.SetRequestContent(newRequestContent);
    if(request.GetHeader("content-type").find("multipart/form-data") != std::string::npos)
        request.SetBoundaryKey(extractBoundary(request.GetHeader("content-type")));
    
    request.ReadAllDataFromSocket(true);
}

RequestResult HttpController::ParseBody(Request& request)
{
    int contentLength = std::atoi(request.GetHeader("content-length").c_str());
    request.SetBody(request.GetRequestContent().substr(0, contentLength));
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
    // if(!_requests[socketId].ReadAllDataFromSocket())
    // {
        while(true)
        {
            char requestBuffer[MessageBuffer];
            requestResult = Tools::Recv(socketId, requestBuffer);
            if(requestResult != Success)
                return requestResult;
            if(std::string(requestBuffer).length() == 0)
                break;
            _requests[socketId].AppendRequestContent(std::string(requestBuffer));
        }
        // std::cout<<_requests[socketId]._requestContent<<std::endl;

        // std::cout<<"length: "<<_requests[socketId]._requestContent.length()<<std::endl;
        // std::cout<<"Content"<<std::endl;
        // std::cout<<_requests[socketId]._requestContent<<std::endl;
        // if(_requests[socketId]._requestContent == "")
            // return Success;

        if(!_requests[socketId].ReadAllDataFromSocket())
            ParseRequestHeaders(_requests[socketId]);
    // }

        // Content-Disposition: form-data; name="file"; filename="Makefile"
        // Content-Type: application/octet-stream

    // std::cout<<_requests[socketId].GetRequestContent()<<std::endl;

    // if(_requests[socketId].HasHeader("content-length"))
        // requestResult = ParseBody(_requests[socketId]);
    // else if(_requests[socketId].HasHeader("transfer-encoding") &&
        // !_requests[socketId].GetHeader("transfer-encoding").compare("chunked"))
        // requestResult = ParseChunked(_requests[socketId]);

    // std::cout<<"First Time\n\n"<<std::endl;
    // ------WebKitFormBoundaryMFSzWIVmPfUhCZUK--
    // ------WebKitFormBoundaryMFSzWIVmPfUhCZUK--

    std::string boundaryKey = _requests[socketId].GetBoundaryKey();
    // std::cout<<"BoundaryKey: "<<boundaryKey<<std::endl;
    std::cout<<_requests[socketId]._requestContent<<std::endl;
    std::cout<<std::endl;
    if(boundaryKey != "")
    {
        // if(_requests[socketId].GetRequestContent().rfind(_requests[socketId].GetBoundaryKey()) != std::string::npos)
        //     requestResult = Success;
        requestResult = Multipart;
    }
    // _requests[socketId]._requestContent.clear();
    return requestResult;
}
RequestResult HttpController::HttpRequest(int readSocket)
{
    if(!CheckRequestIn(readSocket))
        CreateNewRequest(readSocket);

    RequestResult requestResult = ProcessHTTPRequest(readSocket);
    if(requestResult == ReadError || requestResult == ClosedConnection)
        _requests.erase(readSocket);

    // std::cout<<_requests[readSocket].GetMethod()<<std::endl;
    // std::cout<<_requests[readSocket].GetPath()<<std::endl;
    // std::cout<<_requests[readSocket].GetVersion()<<std::endl;
    // std::cout<<"print Headers"<<std::endl;
    // _requests[readSocket].printMap();
    // std::cout<<_requests[readSocket].GetBody()<<std::endl;
    // std::cout<<_requests[readSocket].GetChunk()<<std::endl;
    // std::cout<<_requests[readSocket].GetChunkSize()<<std::endl;

    return requestResult;
}

void HttpController::HttpResponse(int readSocket) //* Needs some work *
{
    if(_requests[readSocket].GetRequestContent().length() > LimitRequest)
    {
        // std::cout<<"Code: 413"<<std::endl;
        const char *http_response = "HTTP/1.1 413 Content Too Large\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<title>Welcome to nginx!</title>\n<style>\nhtml { color-scheme: light dark; }\nbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n</style>\n</head>\n<body>\n<h1>Welcome to nginx!</h1>\n<p>If you see this page, the nginx web server is successfully installed and working. Further configuration is required.</p>\n<p>For online documentation and support please refer to <a href=\"http://nginx.org/\">nginx.org</a>.<br/>Commercial support is available at <a href=\"http://nginx.com/\">nginx.com</a>.</p>\n<p><em>Thank you for using nginx.</em></p>\n</body>\n</html>";
        std::cout<<"Send: "<<send(readSocket, http_response, strlen(http_response), 0)<<std::endl;
    }
    else
    {   
    // std::cout << "Code: 200" << std::endl;
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
    "background-color: #f0f0f0;\n"
    "}\n"
    ".container {\n"
    "text-align: center;\n"
    "}\n"
    "h1 {\n"
    "color: #333;\n"
    "}\n"
    "p {\n"
    "color: #666;\n"
    "}\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<div class=\"container\">\n"
    "<h1>\n"
    "This is a simple HTML file.\n"
    "</h1>\n"
    "<p>\n"
    "Feel free to modify and add your content here!\n"
    "</p>\n"
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
    "e.preventDefault(); // Prevent default form submission\n"
    "const formData = new FormData(form); // Create FormData object from the form\n"
    "try {\n"
    "const response = await fetch('/upload', {\n"
    "method: 'POST',\n"
    "body: formData,\n"
    "});\n"
    "if (!response.ok) {\n"
    "throw new Error('Upload failed');\n"
    "}\n"
    "statusMessage.textContent = 'File uploaded successfully!';\n"
    "form.reset(); // Reset the form after successful upload\n"
    "} catch (error) {\n"
    "statusMessage.textContent = 'Upload failed. Please try again.';\n"
    "console.error('Error:', error);\n"
    "}\n"
    "});\n"
    "});\n"
    "</script>\n"
    "</body>\n"
    "</html>";

    // const char* http_response =
    // "HTTP/1.1 200 OK\r\n"
    // "Content-Type: text/html\r\n\r\n"
    // "<!DOCTYPE html>\n"
    // "<html>\n"
    // "<head>\n"
    // "<title>Miracle</title>\n"
    // "<style>\n"
    // "html { color-scheme: light dark; }\n"
    // "body { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n"
    // "</style>\n"
    // "</head>\n"
    // "<body>\n"
    // "<h1>Welcome to Miracle!</h1>\n"
    // "<p>If you see this page, the Miracle web server is successfully installed and\n"
    // "working. Further configuration is required.</p>\n"
    // "<p><em>Thank you for using Miracle.</em></p>\n"
    // "</body>\n"
    // "</html>";

    // const char* http_response =
    // "HTTP/1.1 200 OK\r\n"
    // "Content-Type: text/html\r\n\r\n"
    // "<!DOCTYPE html>\n"
    // "<html lang=\"en\">\n"
    // "<head>\n"
    // "<meta charset=\"UTF-8\">\n"
    // "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    // "<title>Change Background Color</title>\n"
    // "<style>\n"
    // "body {\n"
    // "text-align: center;\n"
    // "padding-top: 50px;\n"
    // "font-family: Arial, sans-serif;\n"
    // "transition: background-color 0.3s ease;\n"
    // "}\n"
    // "h2 {\n"
    // "transition: color 0.3s ease;\n"
    // "}\n"
    // ".button-6 {\n"
    // "align-items: center;\n"
    // "background-color: #FFFFFF;\n"
    // "border: 1px solid rgba(0, 0, 0, 0.1);\n"
    // "border-radius: .25rem;\n"
    // "box-shadow: rgba(0, 0, 0, 0.02) 0 1px 3px 0;\n"
    // "box-sizing: border-box;\n"
    // "color: rgba(0, 0, 0, 0.85);\n"
    // "cursor: pointer;\n"
    // "display: inline-flex;\n"
    // "font-family: system-ui, -apple-system, system-ui, \"Helvetica Neue\", Helvetica, Arial, sans-serif;\n"
    // "font-size: 16px;\n"
    // "font-weight: 600;\n"
    // "justify-content: center;\n"
    // "line-height: 1.25;\n"
    // "margin: 0;\n"
    // "min-height: 3rem;\n"
    // "padding: calc(.875rem - 1px) calc(1.5rem - 1px);\n"
    // "position: relative;\n"
    // "text-decoration: none;\n"
    // "transition: all 250ms;\n"
    // "user-select: none;\n"
    // "-webkit-user-select: none;\n"
    // "touch-action: manipulation;\n"
    // "vertical-align: baseline;\n"
    // "width: auto;\n"
    // "}\n"
    // ".button-6:hover,\n"
    // ".button-6:focus {\n"
    // "border-color: rgba(0, 0, 0, 0.15);\n"
    // "box-shadow: rgba(0, 0, 0, 0.1) 0 4px 12px;\n"
    // "color: rgba(0, 0, 0, 0.65);\n"
    // "}\n"
    // ".button-6:hover {\n"
    // "transform: translateY(-1px);\n"
    // "}\n"
    // ".button-6:active {\n"
    // "background-color: #F0F0F1;\n"
    // "border-color: rgba(0, 0, 0, 0.15);\n"
    // "box-shadow: rgba(0, 0, 0, 0.06) 0 2px 4px;\n"
    // "color: rgba(0, 0, 0, 0.65);\n"
    // "transform: translateY(0);\n"
    // "}\n"
    // "</style>\n"
    // "</head>\n"
    // "<body>\n"
    // "<h2>Select Background Color</h2>\n"
    // "<button class=\"button-6\" role=\"button\" onclick=\"changeColor('black')\">Black</button>\n"
    // "<button class=\"button-6\" role=\"button\" onclick=\"changeColor('white')\">White</button>\n"
    // "<button class=\"button-6\" role=\"button\" onclick=\"changeColor('pink')\">Pink</button>\n"
    // "<button class=\"button-6\" role=\"button\" onclick=\"changeColor('violet')\">Violet</button>\n"
    // "<script>\n"
    // "function changeColor(color) {\n"
    // "document.body.style.backgroundColor = color;\n"
    // "if (color === 'black' || color === 'violet') {\n"
    // "document.querySelector('h2').style.color = 'white';\n"
    // "} else {\n"
    // "document.querySelector('h2').style.color = 'black';\n"
    // "}\n"
    // "}\n"
    // "</script>\n"
    // "</body>\n"
    // "</html>";


    std::cout << "Send: " << send(readSocket, http_response, strlen(http_response), 0) << std::endl;
    }
    _requests.erase(readSocket);
}