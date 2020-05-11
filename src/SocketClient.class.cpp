#include "SocketClient.class.hpp"
#include "IRCMessage.class.hpp"
#include <iostream>
#include "Utility.hpp"
#include <time.h>
#include <regex>
#include "IRCServer.class.hpp"
SocketClient::SocketClient(void)
{
    return;
}

SocketClient::SocketClient(int const socket, sockaddr_in const &addr) : Socket(socket, addr)
{
    return;
}

SocketClient::~SocketClient(void)
{
}

bool SocketClient::recvMessage() {
    char buffer[BUFF_MSG] = {0};
    int sckt = this->getSocket();
    int ret = recv(sckt, buffer, BUFF_MSG, 0);
    if (ret == 0)
    {
        std::cout << "Connexion terminee" << std::endl;
        IRCServer::_client_manager.deleteClient(this, ClientManager::ClientChoice::USER);
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
        //Todo decouper les buffers par CR-LF
        message.erase(message.size() - 1);
        IRCMessage IRC_message(message);
        if (IRC_message.isValid())
            IRCServer::_message_mediator.handleMessage(IRC_message, this);
    }
    return (false);
}

void SocketClient::handle(SocketManagerInterface &dispatcher)
{
    dispatcher.dispatch(*this);
}

