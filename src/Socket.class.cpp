#include "Socket.class.hpp"
#include <cerrno>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <arpa/inet.h>

Socket::Socket(void) : _socket(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)),
                       _addr()
{
    this->checkSocket();
    return;
}

Socket::Socket(int const socket, sockaddr_in const &addr) : _socket(socket),
                                                            _addr(addr)
{
    this->checkSocket();
    return;
}

Socket::~Socket(void)
{
    if (this->_socket != -1)
        close(this->_socket);
}

int Socket::getSocket() const
{
    return (this->_socket);
}

void Socket::checkSocket() const
{
    if (this->_socket == -1)
        throw std::runtime_error(std::strerror(errno));
}

std::string Socket::getAddr() const
{
    char buff[INET6_ADDRSTRLEN];
    return inet_ntop(this->_addr.sin_family, &(this->_addr.sin_addr), buff, INET6_ADDRSTRLEN);
}

Socket &Socket::setAddr(sockaddr_in const &addr)
{
    this->_addr = addr;
    return (*this);
}

unsigned int    Socket::getPort() const
{
    return ntohs(this->_addr.sin_port);
}