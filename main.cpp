#include "Client.hpp"

int findMatchingServerBlock(std::vector<ServerConfig> &server_config, const std::string &host)
{
	for (std::vector<ServerConfig>::iterator it = server_config.begin();
		 it != server_config.end(); ++it)
	{
		for (std::vector<std::string>::const_iterator it2 = it->getServerName().begin();
			 it2 != it->getServerName().end(); ++it2)
		{
			if (host == *it2)
				return it - server_config.begin();
		}
	}
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

std::string printAddr(const in_addr &ip, const in_port_t &port)
{
	std::ostringstream oss;
	oss << inet_ntoa(ip) << ":" << ntohs(port);
	return oss.str();
}

void printListeningSockets(std::vector<Socket> &sockets)
{
	for (std::vector<Socket>::iterator it = sockets.begin();
		 it != sockets.end(); ++it)
	{
		std::cout << "Server listening on ip " << printAddr(it->getAddr().sin_addr, 
			it->getAddr().sin_port) << std::endl;
		for (std::vector<ServerConfig>::iterator it2 = it->serverBlocks.begin();
			 it2 != it->serverBlocks.end(); ++it2)
		{
			if (it2->getServerName().empty())
				std::cout << "server_name: " << std::endl;

			for (std::vector<std::string>::const_iterator it3 = it2->getServerName().begin();
				 it3 != it2->getServerName().end(); ++it3)
			{
				std::cout << "server_name: " << *it3 << std::endl;
			}
		}
	}
}

int castIpAndPort(sockaddr_in &serverAddr, const std::pair<std::string, ssize_t> &pair)
{
	const char *ip_str = pair.first.c_str();
	in_addr_t ip_addr = inet_addr(ip_str);
	if (ip_addr == INADDR_NONE)
	{
		std::cerr << "Invalid IP address" << std::endl;
		return 1;
	}
	serverAddr.sin_addr.s_addr = ip_addr;

	ssize_t portSizeT = pair.second;
	if (portSizeT < 0 || portSizeT > UINT16_MAX)
	{
		std::cerr << "Invalid port number" << std::endl;
		return 1;
	}
	serverAddr.sin_port = htons(portSizeT);
	return 0;
}

void closeSockets(std::vector<Socket> &sockets)
{
	for (std::vector<Socket>::iterator it = sockets.begin();
		 it != sockets.end(); ++it)
	{
		close(it->getFd());
	}
}

bool socketExists(std::vector<Socket> &sockets, sockaddr_in &addr, ServerConfig &serverBlock)
{
	for (std::vector<Socket>::iterator it = sockets.begin();
		 it != sockets.end(); ++it)
	{
		if (it->getAddr().sin_addr.s_addr == addr.sin_addr.s_addr && it->getAddr().sin_port == addr.sin_port)
		{
			it->serverBlocks.push_back(serverBlock);
			return true;
		}
	}
	return false;
}

int createListeningSockets(std::vector<ServerConfig> &server_config, std::vector<Socket> &sockets,
						   int &num, fd_set &masterRead)
{
	for (std::vector<ServerConfig>::iterator it = server_config.begin();
		 it != server_config.end(); ++it)
	{
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		if (castIpAndPort(serverAddr, it->getListen()))
			return 1;
		if (socketExists(sockets, serverAddr, *it))
			continue;
		int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket == -1)
			return 1;
		Socket socket;
		socket.setAddr(serverAddr);
		socket.setFd(serverSocket);
		socket.serverBlocks.push_back(*it);
		sockets.push_back(socket);
		int enable = 1;
		if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1 ||
			setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1 ||
			bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1 ||
			listen(serverSocket, MAX_SOCK_CONNECTIONS) == -1)
			return 1;
		FD_SET(serverSocket, &masterRead);
		if (serverSocket >= num)
			num = serverSocket + 1;
	}
	return 0;
}

void buildResponse(Client &client)
{
	int i;
	if (client.request->getHeaders().find("host") == client.request->getHeaders().end())
		i = 0;
	else
		i = findMatchingServerBlock(client.getSocket().serverBlocks,
									client.request->getHeaders().at("host"));
	client.response = new Response(*client.request, client.getSocket().serverBlocks[i]);
	client.response->buildResponse();
}

int readRequest(Client &client)
{
	char buff[BUFFSIZE + 1];
	memset(buff, 0, BUFFSIZE + 1);
	ssize_t bytesRead;

	bytesRead = recv(client.getFd(), buff, BUFFSIZE, 0);
	if (bytesRead <= 0)
		return 1;
	std::string chunk(buff, bytesRead);
	if (client.request->parse(chunk))
		client.request->setStatus(DONE);
	if (client.request->isReadComplete())
	{
		std::cout << "\033[1;32m" << "Request: " << client.request->getMethod() << std::endl;
		std::cout << "URL: " << client.request->getPath() << "\033[0m" << std::endl;
	}
	return 0;
}

std::map<int, Client *>::iterator removeClient(std::map<int, Client *>::iterator it, std::map<int, Client*> &clients, fd_set &set)
{
	std::cout << "Client " << printAddr(clients[it->first]->getAddr().sin_addr,
		clients[it->first]->getAddr().sin_port) << " disconnected" << std::endl;
	FD_CLR(it->first, &set);
	delete clients[it->first];
	close(it->first);
	return clients.erase(it);
}

void createClientConnection(Socket &socket, std::map<int, Client*> &clients,
						   fd_set &masterRead)
{
	sockaddr_in clientAddr;
	socklen_t addr_length = sizeof(clientAddr);
	int clientSocket = accept(socket.getFd(),
							  reinterpret_cast<sockaddr *>(&clientAddr), &addr_length);
	if (clientSocket == -1 || fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "Could not connect to the client from " << 
			printAddr(clientAddr.sin_addr, clientAddr.sin_port) << std::endl;
		return;
	}
	std::cout << "New connection from " << 
		printAddr(clientAddr.sin_addr, clientAddr.sin_port) << std::endl;
	FD_SET(clientSocket, &masterRead);
	Client *client = new Client(clientSocket, socket);
	client->setAddr(clientAddr);
	clients[clientSocket] = client;
}

int runServers(std::vector<Socket> &sockets, fd_set &masterRead, int numSock)
{
	fd_set masterWrite;
	fd_set fdread;
	fd_set fdwrite;
	FD_ZERO(&masterWrite);
	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);
	std::map<int, Client *> clients;

	int num;
	while (true)
	{
		fdread = masterRead;
		fdwrite = masterWrite;
		num = clients.empty() ? numSock : clients.rbegin()->first + 1;
		if (select(num, &fdread, &fdwrite, NULL, NULL) < 0)
		{
			std::cerr << "select() error" << std::endl;
			return 1;
		}
		for (std::vector<Socket>::iterator it = sockets.begin();
			 it != sockets.end(); ++it)
		{
			if (FD_ISSET(it->getFd(), &fdread))
				createClientConnection(*it, clients, masterRead);
		}
		if (clients.empty())
			continue;
		for (std::map<int, Client *>::iterator it = clients.begin();
			 it != clients.end(); ++it)
		{
			if (FD_ISSET(it->first, &fdread))
			{
				Request *request = it->second->request;
				Response *response = it->second->response;
				if (!request->isReadComplete() && readRequest(*it->second))
				{
					it = removeClient(it, clients, masterRead);
					if (it == clients.end())
				        break;
					else
						continue;
				}
				if (request->isReadComplete() && !response)
					buildResponse(*it->second);
				response = it->second->response;
				if (request->isReadComplete() && response->isReady())
				{
					FD_SET(it->first, &masterWrite);
					FD_CLR(it->first, &masterRead);
				}
			}
		}
		for (std::map<int, Client *>::iterator it = clients.begin();
			 it != clients.end(); ++it)
		{
			if (FD_ISSET(it->first, &fdwrite))
			{
				Response *response = it->second->response;
				if (response->sendResponse(it->second->getFd()))
					it = removeClient(it, clients, masterWrite);
				if (response->isSent())
					it = removeClient(it, clients, masterWrite);
				if (it == clients.end())
				        break;
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	// Validate arguments
	if (validateArgc(argc))
		return 1;
	// Parsing config and assigning values
	static std::vector<ServerConfig> server_config;
	if (createServerConfig(argc, argv, server_config))
		return 1;

	int num = 1;
	std::vector<Socket> sockets;
	fd_set masterRead;
	FD_ZERO(&masterRead);
	if (createListeningSockets(server_config, sockets, num, masterRead))
	{
		std::cout << "Error creating listening sockets" << std::endl;
		closeSockets(sockets);
		return 1;
	}
	printListeningSockets(sockets);

	if (runServers(sockets, masterRead, num))
	{
		std::cerr << "Error running server" << std::endl;
		return 1;
	}
	return 0;
}
