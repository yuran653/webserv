#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <fcntl.h>

#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

#define BUFFSIZE 10
#define PORT 8080

int clientSocketGlobal;

void signalHandler(int signal)
{
	std::cout << "Caught Ctrl+C. Closing client fd" << std::endl;
	close(clientSocketGlobal);
	exit(signal);
}

void handleRequest(int clientSocket)
{
	char buff[BUFFSIZE] = {0};
	ssize_t bytesRead;
	Request *request = new Request();
	Response response;
	ServerConfig config;

	// Ctrl+C sudden exit handling to close client socket
	clientSocketGlobal = clientSocket;
	signal(SIGINT, signalHandler);

	// set the socket to non-blocking mode
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0 ) {
    	perror("Error setting socket to non-blocking mode");
		close(clientSocket);
   		return;
	}

	std::string saveRequest;
	while (true) 
	{
		bytesRead = recv(clientSocket, buff, sizeof(buff), 0);
		if (bytesRead < 0) 
		{
			// Handle non-blocking error.
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// No data available, continue the loop.
				continue;
			} else {
				// Other error, handle appropriately.
				perror("Error reading from socket");
				close(clientSocket);
				break;
       		}
		}
		if (bytesRead < BUFFSIZE)
			request->setReadComplete(true);
		std::string chunk(buff, bytesRead);
		saveRequest.append(buff, bytesRead);
		if (request->parse(chunk) || request->isParsingComplete())
			break;
		memset(buff, 0, sizeof(buff));
	}
	std::cout << *request << std::endl;
	size_t found = 0;
    while ((found = saveRequest.find("\r\n", found)) != std::string::npos) {
        saveRequest.replace(found, 2, "\\r\\n");
        found += 4;  // Move past the four characters added
    }
	std::cout << saveRequest << std::endl;
	response.request = *request;
	response.setConfig(config);
	response.buildResponse();
	std::cout << response.getResponse() << std::endl;
	send(clientSocket, response.getResponse().c_str(), response.getResponse().length(), 0);
	close(clientSocket);
}


// simple running webserver
int main()
{

	// Create socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "Failed to create socket\n";
		return EXIT_FAILURE;
	}

	int enable = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

	// Bind to port
	sockaddr_in serverAddr;//{}; <----
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1)
	{
		std::cerr << "Failed to bind to port " << PORT << "\n";
		close(serverSocket);
		return EXIT_FAILURE;
	}

	// Listen for incoming connections
	if (listen(serverSocket, SOMAXCONN) == -1)
	{
		std::cerr << "Failed to listen for connections\n";
		close(serverSocket);
		return EXIT_FAILURE;
	}

	std::cout << "Server listening on port " << PORT << "\n";

	while (true)
	{
		sockaddr_in clientAddr;//{}; <----
		socklen_t clientAddrLen = sizeof(clientAddr);

		// Accept connection
		int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
		if (clientSocket == -1)
		{
			std::cerr << "Failed to accept connection\n";
			continue;
		}

		// Handle request
		handleRequest(clientSocket);
	}

	return 0;
}
