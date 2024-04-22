#include "Request.hpp"

Request::Request() :
    _method(NONE),
    _path(""),
    _version(""),
    _port(0),
    _headers(),
    _env_for_cgi(),
    _body("") {
}

void Request::SetMethod(HttpMethod method) { _method = method; }
void Request::SetPath(std::string& path) { _path = path; }
void Request::SetVersion(std::string& version) { _version = version; }

HttpMethod Request::GetMethod(void) const { return _method; }
const std::string& Request::GetPath(void) const { return _path; }
const std::string& Request::GetVersion(void) const { return _version; }