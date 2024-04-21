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
