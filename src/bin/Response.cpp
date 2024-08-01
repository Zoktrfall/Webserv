#include "Response.hpp"

Response::Response(void) :
    _statusCode(-1),
    _reasonPhrase(""),
    _responseBody("") {
        _headers["Server"] = "";
        _headers["Date"] = "";
        _headers["Content-Length"] = "";
        _headers["Content-Type"] = "";
        _headers["Content-Language"] = "";
        _headers["Connection"] = "";
        _headers["Set-Cookie"] = "";
        _headers["Allow"] = "";
        _headers["Content-Location"] = "";
        _headers["Location"] = "";
        _headers["Retry-After"] = "";
}

short Response::GetStatusCode(void) const { return _statusCode; }
std::string Response::GetReasonPhrase(void) const { return _reasonPhrase; }
std::map<std::string, std::string> Response::GetHeaders(void) const { return _headers; }
std::string Response::GetResponseBody(void) const { return _responseBody; } 

void Response::SetStatusCode(short statusCode) { _statusCode = statusCode; }
void Response::SetReasonPhrase(const std::string& reasonPhrase) { _reasonPhrase = reasonPhrase; }
void Response::SetHeader(const std::string& headerName, const std::string& headerValue) { _headers[headerName] = headerValue; }
void Response::SetResponseBody(const std::string& responseBody) { _responseBody = responseBody; }
void Response::Clear(void)
{
    _responseBody.clear();
    _statusCode = 0;
    _reasonPhrase.clear();
    _headers.clear();
}