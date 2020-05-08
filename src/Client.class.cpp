#include "Client.class.hpp"


Client::Client(SocketClient *socket_client) : _socket_client(socket_client), status(Status::CONNECTING) {
    return ;
}

Client::~Client(void) {
    return ;
}

std::string Client::getName() const {
    return (this->_name);
}

SocketClient * Client::getSocketClient(void) const{
    return this->_socket_client;
}

