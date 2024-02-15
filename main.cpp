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
#include "ServerConfig.hpp"
#include "Location.hpp"

void handleRequest(int clientSocket, const ServerConfig &config)
{
	char *buff = new char[BUFFSIZE];
	memset(buff, 0, BUFFSIZE);
	ssize_t bytesRead;
	Request *request = new Request();
	Response response;

	// set the socket to non-blocking mode
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("Error setting socket to non-blocking mode");
		close(clientSocket);
		return;
	}

	while (true)
	{
		bytesRead = recv(clientSocket, buff, BUFFSIZE, 0);
		if (bytesRead < 0)
		{
			// Handle non-blocking error.
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				// No data available, continue the loop.
				continue;
			}
			else
			{
				// Other error, handle appropriately.
				perror("Error reading from socket");
				close(clientSocket);
				break;
			}
		}
		else if (bytesRead == 0)
		{
			close(clientSocket);
			return;
		}
		// if (bytesRead == 0)
		// 	request->setReadComplete(true);
		std::string chunk(buff, bytesRead);
		if (request->parse(chunk) || request->isParsingComplete())
			break;
		memset(buff, 0, BUFFSIZE);
	}
	std::cout << *request << std::endl;
	// size_t found = 0;
	// while ((found = saveRequest.find("\r\n", found)) != std::string::npos) {
	//     saveRequest.replace(found, 2, "\\r\\n");
	//     found += 4;  // Move past the four characters added
	// }
	// std::cout << saveRequest << std::endl;
	response.request = *request;
	response.setConfig(config);
	response.buildResponse();
	// std::cout << response.getResponse() << std::endl;
	send(clientSocket, response.getResponse().c_str(), response.getResponse().length(), 0);
	close(clientSocket);
	delete request;
	delete[] buff;
}

int printServerConfig(const std::vector<ServerConfig> &server_config)
{
	// ---------------------------------------------------------------- //
	std::cout << "NUMBER OF SERVERS: " << server_config.size() << std::endl
			  << std::endl;
	// ---------------------------------------------------------------- //
	for (size_t i = 0; i < server_config.size(); i++)
	{
		std::cout << "SERVER[" << i + 1 << "]:" << std::endl;
		if (server_config.at(i).getServerBlock().empty() == false)
		{
			std::cerr << "Error: temporary string is not empty" << std::endl;
			return 1;
		}
		if (server_config.at(i).getDefaultServer() == true)
			std::cout << "-> DEFAULT SERVER: ON" << std::endl;
		else
			std::cout << "-> DEFAULT SERVER: OFF" << std::endl;
		std::cout << "-> LISTEN: [" << server_config.at(i).getListen().first << ":"
				  << server_config.at(i).getListen().second << "]" << std::endl;
		std::cout << "-> SERVER NAME: ";
		for (size_t j = 0; j < server_config.at(i).getServerName().size(); j++)
			std::cout << "[" << server_config.at(i).getServerName().at(j) << "]";
		std::cout << std::endl;
		std::map<std::string, Location> location_map = server_config.at(i).getLocationMap();
		for (std::map<std::string, Location>::iterator it = location_map.begin(); it != location_map.end(); it++)
		{
			std::cout << "-> LOCATION: [" << it->first << "]" << std::endl;
			if (it->second.getAutoindex() == true)
				std::cout << "  -> AUTOINDEX: [ON]" << std::endl;
			else
				std::cout << "  -> AUTOINDEX: OFF" << std::endl;
			std::cout << "  -> ROOT: [" << it->second.getRoot() << "]" << std::endl;
			std::cout << "  -> INDEX: ";
			for (size_t j = 0; j < it->second.getIndex().size(); j++)
				std::cout << "[" << it->second.getIndex().at(j) << "]";
			std::cout << std::endl;
			std::cout << "  -> LIMIT EXCEPT: ";
			for (std::set<std::string>::iterator it_set = it->second.getLimitExcept().begin();
				 it_set != it->second.getLimitExcept().end(); it_set++)
				std::cout << "[" << *it_set << "]";
			std::cout << std::endl;
			std::cout << "  -> RETURN: [" << it->second.getReturn().first << "] -> ["
					  << it->second.getReturn().second << "]" << std::endl;
			std::cout << "  -> CGI PATH: [" << it->second.getCgiPass() << "]" << std::endl;
			std::cout << "  -> CLIENT BODY TMEP PATH: [" << it->second.getClientBodyTempPath() << "]" << std::endl;
			std::cout << "  -> CLIENT MAX SIZE: [" << it->second.getClientMaxBodySize() << "]" << std::endl;
		}
		std::map<int, std::string> error_page = server_config.at(i).getErrorPage();
		for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); it++)
			std::cout << "-> ERROR CODE: [" << it->first << "] ERROR PATH: [" << it->second << "]" << std::endl;
		std::cout << std::endl;
	}
	// ---------------------------------------------------------------- //
	std::cout << "->OK<-" << std::endl;
	// ----------------------------------------------------- ----------- //
	return 0;
}

int createServerConfig(int argc, char *argv[], std::vector<ServerConfig> &server_config)
{
	std::string config_name(DEFAULT);
	if (argc == 2)
		config_name = std::string(argv[1]);
	try
	{
		Config::createServerConfig(config_name, server_config);
	}
	catch (const Config::ReadConfigFileError &e)
	{
		std::cerr << RED "Error: " RESET_RED << e.what() << std::endl;
		return 1;
	}
	return 0;
}

int validateArgc(int argc)
{
	if (argc > 2)
	{
		std::cerr << RED "Error:" RESET_RED " Usage: ./webserv or ./webserv [config_file_name].conf" << std::endl;
		return 1;
	}
	return 0;
}

// simple running webserver
int main(int argc, char *argv[])
{
	// Validate arguments
	if (validateArgc(argc))
		return 1;
	// Parsing config and assigning values
	static std::vector<ServerConfig> server_config;
	if (createServerConfig(argc, argv, server_config))
		return 1;
	// Printing config values
	// if (printServerConfig(server_config))
	// 	return 1;

	// -----> * SERVER PART STARTS HERE * < -----

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
	sockaddr_in serverAddr; //{}; <----
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
		sockaddr_in clientAddr; //{}; <----
		socklen_t clientAddrLen = sizeof(clientAddr);

		// Accept connection
		int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
		if (clientSocket == -1)
		{
			std::cerr << "Failed to accept connection\n";
			continue;
		}

		// Handle request
		handleRequest(clientSocket, server_config.at(1));
	}

	return 0;
}
