#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <vector>
#include "ServerConfig.hpp"

class Socket
{
	private:
		int _fd;
		sockaddr_in _addr;

	public:
		void setAddr(sockaddr_in addr);
		void setFd(int fd);
		int getFd() const;
		sockaddr_in getAddr() const;
		std::vector<ServerConfig> serverBlocks;
};

#endif