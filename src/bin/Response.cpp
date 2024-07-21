#include "Response.hpp"

Response::Response(void) :
    _response(""), 
    _statusCode(-1),
    _reasonPhrase("") {
        _headers["Server"] = "";
        _headers["Date"] = "";
        _headers["Content-Length"] = "";
        _headers["Content-Type"] = "";
        _headers["Content-Encoding"] = "";
        _headers["Content-Language"] = "";
        _headers["Connection"] = "";
        _headers["Cache-Control"] = "";
        _headers["Set-Cookie"] = "";
        _headers["Allow"] = "";
        _headers["Content-Location"] = "";
        _headers["Location"] = "";
        _headers["Retry-After"] = "";
}

