#include "Client.hpp"

Client::Client(int fd, Socket &socket) : _fd(fd), _socket(socket) {
	request = new Request();
	response = NULL;
}

Client::~Client() {
	if (response)
		delete response;
	if (request)
		delete request;
}

int Client::getFd() const {
	(void)_socket; // for compiler
	return _fd;
}

Socket &Client::getSocket() const
{
	return _socket;
}

void Client::setAddr(sockaddr_in addr)
{
	_addr.sin_len = addr.sin_len;
	_addr.sin_family = addr.sin_family;
	_addr.sin_port = addr.sin_port;
	_addr.sin_addr.s_addr = addr.sin_addr.s_addr;
	memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));
}

sockaddr_in Client::getAddr() const
{
	return _addr;
}