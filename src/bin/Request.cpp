#include "Request.hpp"

Request::Request() :
    _requestContent(""), 
    _method(NONE),
    _path(""),
    _version(""),
    _headers(),
    _body(""),
    _chunks("") {
        _headersRead = false;
        _firstLineCheck = false;
        _requestComplete = false;
        _chunkSize = -1;
}

void Request::SetFirstLineCheck(bool firstLineCheck) { _firstLineCheck = firstLineCheck; }
void Request::ReadFurther(bool requestComplete) { _requestComplete = requestComplete; }
void Request::AreHeadersFinished(bool headersRead) { _headersRead = headersRead; }
void Request::AppendRequestContent(std::string partRequestContent) { _requestContent += partRequestContent; }
void Request::SetRequestContent(std::string& requestContent) { _requestContent = requestContent; }

void Request::SetSocketId(const int socketId) { _socketId = socketId; }
void Request::SetMethod(HttpMethod method) { _method = method; }
void Request::SetPath(std::string& path) { _path = path; }
void Request::SetVersion(std::string& version) { _version = version; }
void Request::SetHeader(const std::string& headerName, const std::string& headerValue) { _headers[headerName] = headerValue; }
void Request::SetBody(const std::string& partBody) { _body += partBody; }
void Request::SetChunk(const std::string& chunk) { _chunks += chunk; }
void Request::SetChunkSize(const int chunkSize) { _chunkSize = chunkSize; }


bool Request::GetFirstLineCheck(void) const { return _firstLineCheck; }
bool Request::ReadFurther(void) const { return _requestComplete; }
bool Request::AreHeadersFinished(void) const { return _headersRead; }
const std::string& Request::GetRequestContent(void) const { return _requestContent; }

int Request::GetSocketId(void) const { return _socketId; }
HttpMethod Request::GetMethod(void) const { return _method; }
const std::string& Request::GetPath(void) const { return _path; }
const std::string& Request::GetVersion(void) const { return _version; }
bool Request::HasHeader(const std::string& headerName) const { return _headers.find(headerName) != _headers.end(); }
const std::string& Request::GetHeader(const std::string& headerName) { return _headers[headerName]; }
const std::string& Request::GetBody(void) const { return _body; }
const std::string& Request::GetChunk(void) const { return _chunks; }
int Request::GetChunkSize(void) const { return _chunkSize; }