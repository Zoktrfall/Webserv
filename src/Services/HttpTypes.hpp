#ifndef HTTP_TYPES_HPP
#define HTTP_TYPES_HPP
#include <netinet/in.h>
#include <ctime>

enum RequestResult
{
    ClosedConnection,
    ReadError,
    Chunked,
    Multipart,
    Success
};

enum HttpMethod 
{
    GET,
    POST,
    DELETE,
    PUT,
    HEAD,
    NONE
};

struct ServerSocket 
{
    int serverIndex;
    int serverSocket;
    struct sockaddr_in serverAddress;
};

struct ClientSocket 
{
    int serverIndex;
    int clientSocket;
    time_t lastTime;
};

#endif
