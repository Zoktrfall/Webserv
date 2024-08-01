#include "Request.hpp"

Request::Request(void) :
    _requestContent(""), 
    _boundaryKey(""),
    _method(NONE),
    _path(""),
    _version(""),
    _headers(),
    _body(""),
    _chunks("") {
        _chunkSize = -1;
        _firstLineCheck = false;;
}

void Request::SetRequestContent(std::string requestContent) { _requestContent = requestContent; }
void Request::AppendRequestContent(char* partRequestContent, int bytes) { _requestContent.append(partRequestContent, bytes); }
void Request::SetBoundaryKey(std::string boundaryKey) { _boundaryKey = boundaryKey; }
void Request::ClearContent(void) { _requestContent.clear(); }
void Request::FirstLineCheck(bool status) { _firstLineCheck = status; }
bool Request::FirstLineCheck(void) { return _firstLineCheck; }

void Request::SetMethod(HttpMethod method) { _method = method; }
void Request::SetPath(std::string& path) { _path = path; }
void Request::SetVersion(std::string& version) { _version = version; }
void Request::SetHeader(const std::string& headerName, const std::string& headerValue) { _headers[headerName] = headerValue; }
void Request::SetBody(const std::string& partBody) { _body.append(partBody); }
void Request::SetChunk(const std::string& chunk) { _chunks.append(chunk); }
void Request::SetChunkSize(const int chunkSize) { _chunkSize = chunkSize; }


const std::string& Request::GetRequestContent(void) const { return _requestContent; }
const std::string& Request::GetBoundaryKey(void) const { return _boundaryKey; }

HttpMethod Request::GetMethod(void) const { return _method; }
const std::string& Request::GetPath(void) const { return _path; }
const std::string& Request::GetVersion(void) const { return _version; }
bool Request::HasHeader(const std::string& headerName) const { return _headers.find(headerName) != _headers.end(); }
const std::string& Request::GetHeader(const std::string& headerName) { return _headers[headerName]; }
const std::map<std::string, std::string>& Request::GetHeaders(void) const { return _headers; }
const std::string& Request::GetBody(void) const { return _body; }
const std::string& Request::GetChunk(void) const { return _chunks; }
int Request::GetChunkSize(void) const { return _chunkSize; }