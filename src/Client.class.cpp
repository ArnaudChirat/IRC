#include "Client.class.hpp"
#include "IRCMessage.class.hpp"
#include <iostream>
#include "Utility.hpp"
#include "ClientConnecting.class.hpp"
Client::Client(void)
{
    return;
}

Client::Client(int const socket, sockaddr_in const &addr) : Socket(socket, addr)
{
    this->_state = new ClientConnecting(this);
    return;
}

Client::~Client(void)
{
}

bool Client::recvMessage() {
    char buffer[BUFF_MSG] = {0};
    int sckt = this->getSocket();
    int ret = recv(sckt, buffer, BUFF_MSG, 0);
    if (ret == 0)
    {
        std::cout << "Connexion terminee" << std::endl;
        return (true);
    }
    else if (ret == -1)
    {
        std::cout << "Erreur reception : " << std::strerror(errno) << std::endl;
        return (true);
    }
    else
    {
        std::cout << "[" << this->getAddr().c_str() << ":" << this->getPort() << "] " << buffer;
        std::string message(buffer);
        //Todo couper les buffers par CR-LF
        message.erase(message.size() - 1);
        IRCMessage IRCMessage(message);
        std::cout << IRCMessage << std::endl;
        this->_state->handle();
    }
    return (false);
}

void Client::handle(SocketManagerInterface &dispatcher)
{
    dispatcher.dispatch(*this);
}

std::string Client::getNick() const {
    return (this->_nick);
}

std::string Client::getUser() const {
    return (this->_user);
}

Client &Client::setState(IClientState *state)
{
    this->_state = state;
    return (*this);
}