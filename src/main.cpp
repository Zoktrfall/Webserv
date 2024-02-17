#include "server.hpp"

# define PORT 8080


int main(int argc, char const** argv)
{
	(void)	argc;
	(void)	argv;
	getHostIP();
	int		serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	if (serverSocket == -1) {
		std::cerr << "Error while creathing socket" << std::endl;
		return (1);
	}

	sockaddr_in	serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	if (bind(serverSocket, (const sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
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
	return (0);
}