#ifndef HTTP_TYPES_HPP
#define HTTP_TYPES_HPP
#include <netinet/in.h>
#include <ctime>

enum RequestResult
{
    ClosedConnection,
    Chunked,
    Multipart,
    Success
};

enum ThereIs
{
    File,
    Directory,
    NotFound,
    Error
};

enum HttpMethod 
{
    GET,
    POST,
    DELETE,
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

struct ResponseSocket
{
    int serverIndex;
    int responseSocket;
};

#endif
