#include "server.hpp"

int	server(void) {

  getHostIP();
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    if (serverSocket == -1) {
        std::cerr << "Error while creating socket" << std::endl;
        return (1);
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding to address" << std::endl;
        close(serverSocket);
        return (1);
    }

    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        return (1);
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    std::vector<int> clientSockets;

    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        int maxfd = serverSocket;

        for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it) {
            FD_SET(*it, &readfds);
            if (*it > maxfd) {
                maxfd = *it;
            }
        }

        int activity = select(maxfd + 1, &readfds, NULL, NULL, NULL);

        if (activity == -1) {
            std::cerr << "Error in select()" << std::endl;
            break;
        }

        if (FD_ISSET(serverSocket, &readfds)) {
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
            if (clientSocket == -1) {
                std::cerr << "Error accepting connection" << std::endl;
            } else {
                clientSockets.push_back(clientSocket);
            }
        }

        for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ) {
            if (FD_ISSET(*it, &readfds)) {
                // Handle data on *it (client socket)
				std::cout << "--------------------------------" << std::endl << std::endl;
				char buffer[MAX_BUFFER_SIZE];
                int bytesRead = recv(*it, buffer, MAX_BUFFER_SIZE, 0);
                if (bytesRead == -1) {
                    std::cerr << "Error receiving data" << std::endl;
                    close(*it);
                    it = clientSockets.erase(it);
                } else if (bytesRead == 0) {
                    std::cout << "Connection closed by client" << std::endl;
                    close(*it);
                    it = clientSockets.erase(it);
                } else {
                    buffer[bytesRead] = '\0'; // Null-terminate the buffer
                    std::cout << "Received: " << buffer << std::endl;
                    // Handle received data here
                }
				std::cout << "--------------------------------" << std::endl << std::endl;
                // For demonstration purposes, we just close the connection
                close(*it);
                it = clientSockets.erase(it);
            } else {
                ++it;
            }
        }
    }

    close(serverSocket);
    return (0);
}