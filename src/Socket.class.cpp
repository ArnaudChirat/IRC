#include "Socket.class.hpp"
#include "Utility.hpp"
#include <cerrno>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>

Socket::Socket(void) : _socket(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)),
                       _addr()
{
    this->checkSocket();
    return;
}

Socket::Socket(int const socket, sockaddr const & addr) : _socket(socket),
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

std::string Socket::getAddr()
{
    if (this->getPort() == 0)
        return "NONE";
    char buff[INET6_ADDRSTRLEN];
    return inet_ntop(this->_addr.sa_family, Utility::get_in_addr(&this->_addr), buff, INET6_ADDRSTRLEN);
}

void Socket::setAddr(sockaddr const & addr)
{
    this->_addr = addr;
}

unsigned int Socket::getPort()
{
    if (Utility::ipv4(&this->_addr))
        return ntohs((reinterpret_cast<sockaddr_in *>(&this->_addr))->sin_port);
    return ntohs((reinterpret_cast<sockaddr_in6 *>(&this->_addr))->sin6_port);
}

void Socket::readStdin()
{
std::string in;
    std::getline(std::cin, in);
    std::cout << in << std::endl;
}

void Socket::handle(SocketManagerInterface &dispatcher, type t)
{
    if (t == READ)
        dispatcher.dispatch(*this);
}