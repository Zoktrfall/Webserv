#include "HttpResponseController.hpp"

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

void HttpResponseController::HttpResponse(int socketId, Server& Server, Request& Request)
{
    // std::cout<<Request.GetVersion()<<std::endl;
    // std::cout<<Request.GetMethod()<<std::endl;
    // std::cout<<Request.GetPath()<<std::endl;
    // Request.printMap();
    // std::cout<<Request.GetBody()<<std::endl;

    // std::string filename = "./" + Request.GetPath();
    // std::string filename = "./www/html/index.html";
    // std::ifstream file(filename);
    // if (!file.is_open()) {
    //     std::cout<<"Не удалось открыть файл"<<std::endl;
    // }

    // std::stringstream buffer;
    // buffer << file.rdbuf();
    // std::cout<<Tools::ToString(buffer.str().size())<<std::endl;

    // std::string str = "HTTP/1.1 200 OK\r\n"
    //         "Content-Type: text/html\r\n"
    //         "Content-Length: " + Tools::ToString(buffer.str().size()) + "\r\n" +  "Connection: close\r\n"
    //         "\r\n" +
    //         buffer.str();

    // send(socketId, str.c_str(), str.size(), 0);
    // return;



    // if(Request.GetRequestContent().length() > 100000000)
    // {
    //     // std::cout<<"Code: 413"<<std::endl;
    //     const char *http_response = "HTTP/1.1 413 Content Too Large\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<title>Welcome to nginx!</title>\n<style>\nhtml { color-scheme: light dark; }\nbody { width: 35em; margin: 0 auto; font-family: Tahoma, Verdana, Arial, sans-serif; }\n</style>\n</head>\n<body>\n<h1>Welcome to nginx!</h1>\n<p>If you see this page, the nginx web server is successfully installed and working. Further configuration is required.</p>\n<p>For online documentation and support please refer to <a href=\"http://nginx.org/\">nginx.org</a>.<br/>Commercial support is available at <a href=\"http://nginx.com/\">nginx.com</a>.</p>\n<p><em>Thank you for using nginx.</em></p>\n</body>\n</html>";
    //     std::cout<<"Send: "<<send(socketId, http_response, strlen(http_response), 0)<<std::endl;
    // }
   

        std::string filename = ExtractFilename(Request.GetHeader("content-disposition"));
        std::cout<<"filename: "<<filename<<std::endl;
        std::ofstream outFile(filename);
        outFile << Request.GetBody();
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


    std::cout << "Send: " << send(socketId, http_response, strlen(http_response), 0) << std::endl;
}