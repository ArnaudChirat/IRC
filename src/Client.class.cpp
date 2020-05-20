#include "Client.class.hpp"

Client::Client(SocketClient *socket_client) : _socket_client(socket_client), status(Status::CONNECTING)
{
    return;
}

Client::Client(void)
{
    return;
}

Client &Client::operator=(Client const &rhs)
{
    if (this != &rhs)
    {
        this->_socket_client = rhs._socket_client;
        this->_name = rhs._name;
        this->status = rhs.status;
    }
    return (*this);
}

Client::~Client(void)
{
    return;
}

std::string Client::getName() const
{
    return (this->_name);
}

SocketClient *Client::getSocketClient(void) const
{
    return this->_socket_client;
}

void Client::setPassword(std::string const &pass)
{
    // hash password
    this->_password = pass;
}

std::string Client::getPassword() const
{
    return (this->_password);
}