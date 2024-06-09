#include "Location.hpp"

Location::Location(void) :
    _path(""),
    _root(""),
    _autoindex(false),
    _index(""),
    _methods(),
    _return(""),
    _alias(""),
    _client_max_body_size(LimitHeaders),
    _cgi_path(),
    _cgi_ext() {
}