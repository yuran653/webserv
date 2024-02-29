#include "Socket.hpp"

void Socket::setAddr(sockaddr_in addr)
{
	_addr.sin_len = addr.sin_len;
	_addr.sin_family = addr.sin_family;
	_addr.sin_port = addr.sin_port;
	_addr.sin_addr.s_addr = addr.sin_addr.s_addr;
	memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));
}

sockaddr_in Socket::getAddr() const
{
	return _addr;
}

void Socket::setFd(int fd)
{
	_fd = fd;
}

int Socket::getFd() const
{
	return _fd;
}


