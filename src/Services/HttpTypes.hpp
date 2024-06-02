#ifndef HTTP_TYPES_H
#define HTTP_TYPES_H
#include <string>
#include <netinet/in.h>
#include <ctime>

enum RequestResult
{
    ClosedConnection,
    ReadError,
    Chunked,
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
    int serverSocket;
    struct sockaddr_in ServerAddress;
};

struct ClientSocket 
{
    int clientSocket;
    time_t lastTime;
    bool IsChunked;
};

#endif
