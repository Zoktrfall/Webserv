#include "Request.hpp"

Request::Request() :
    _method(NONE),
    _path(""),
    _version(""),
    _headers(),
    _body("") {
        _firstLineCheck = false;
}


void Request::SetFirstLineCheck(bool firstLineCheck) { _firstLineCheck = firstLineCheck; }

void Request::SetMethod(HttpMethod method) { _method = method; }
void Request::SetPath(std::string& path) { _path = path; }
void Request::SetVersion(std::string& version) { _version = version; }
void Request::SetHeader(const std::string& headerName, const std::string& headerValue) { _headers[headerName] = headerValue; }


bool Request::GetFirstLineCheck(void) const { return _firstLineCheck; }

HttpMethod Request::GetMethod(void) const { return _method; }
const std::string& Request::GetPath(void) const { return _path; }
const std::string& Request::GetVersion(void) const { return _version; }