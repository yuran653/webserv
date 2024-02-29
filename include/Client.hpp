#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client
{
	private:
		int _fd;
		sockaddr_in _addr;
		Socket &_socket;

	public:
		Client(int fd, Socket &socket);
		~Client();
		int getFd() const;
		Socket &getSocket() const;
		void setAddr(sockaddr_in addr);
		sockaddr_in getAddr() const;
		Request *request;
		Response *response;
};

#endif