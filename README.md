# Miracle — HTTP Web Server

Miracle is a configurable HTTP web server built from scratch in C++ using low-level TCP socket programming and I/O multiplexing.

The project explores how web servers such as Nginx accept connections, process HTTP requests, serve resources, execute CGI programs, and manage multiple clients without relying on an existing web-server framework.

## Features

- HTTP request parsing and response generation
- Support for multiple simultaneous client connections
- Non-blocking socket communication with I/O multiplexing
- Configurable servers, routes, ports, and behavior
- Static file serving
- GET, POST, and DELETE request handling
- CGI execution
- Chunked request and file uploads
- Cookie and session management
- Custom error pages
- Nginx-style configuration
- Request-size and route-level configuration
- Graceful handling of malformed requests and connection errors

## How It Works

Miracle opens one or more listening sockets based on the supplied configuration file.

Incoming connections are handled through an event-driven loop using I/O multiplexing, allowing the server to manage multiple clients without creating a dedicated thread or process for every connection.

For each request, the server:

1. Accepts and reads data from the client
2. Parses the HTTP request
3. Matches the request to the configured server and route
4. Executes the required operation
5. Generates and sends an HTTP response
6. Keeps or closes the connection depending on the request state

## Technologies

- C++
- HTTP
- TCP/IP
- POSIX sockets
- I/O multiplexing
- CGI
- GNU/Linux
- Make

## Build

Clone the repository:

```bash
git clone https://github.com/Zoktrfall/Webserv.git
cd Webserv
make
