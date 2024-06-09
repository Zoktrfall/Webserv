#include "Server.hpp"

void Server::initErrorPages(void)
{
	_error_pages[301] = "";
	_error_pages[302] = "";
	_error_pages[400] = "";
	_error_pages[401] = "";
	_error_pages[402] = "";
	_error_pages[403] = "";
	_error_pages[404] = "";
	_error_pages[405] = "";
	_error_pages[406] = "";
	_error_pages[500] = "";
	_error_pages[501] = "";
	_error_pages[502] = "";
	_error_pages[503] = "";
	_error_pages[505] = "";
	_error_pages[505] = "";
}
Server::Server(void) :
    _port(0),
    _host(0),
    _server_name(""),
    _root(""),
    _client_max_body_size(LimitRequest),
    _index(""),
    _autoindex(false),
    _locations(),
    _listen_fd(-1) {
        memset(&_server_address, 0, sizeof(_server_address));
        initErrorPages();
}