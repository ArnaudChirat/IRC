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

void    SocketClient::appendToBuffer(std::string const & msg){
    this->_buffer += msg;
}


bool SocketClient::recvMessage() {
    char buffer[BUFF_MSG] = {0};
    int sckt = this->getSocket();
    int ret = recv(sckt, buffer, BUFF_MSG - 1, 0);
    buffer[BUFF_MSG - 1] = '\0';
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
    else if (ret > 1)
    {
        std::cout << "[" << this->getAddr().c_str() << ":" << this->getPort() << "] " << buffer;
        std::string message(buffer);
        //Todo decouper les buffers par CR-LF
        message.erase(message.size() - 1);
        IRCMessage IRC_message(message);
        if (IRC_message.isValid())
            IRCServer::_message_mediator.handleMessage(IRC_message, this);
        else
            std::cout << "not enought params" << std::endl;
    }
    return (false);
}

void SocketClient::handle(SocketManagerInterface &dispatcher, type t)
{
    if (t == READ)
        dispatcher.dispatch(*this);
    if (t == WRITE)
        dispatcher.writeToSocket(*this);
}

bool    SocketClient::sendMessage(){
    int sentBytes;
    while (this->_buffer.size()){
        if ((sentBytes = send(this->getSocket(), this->_buffer.c_str(), this->_buffer.size(), 0)) == -1){
            throw std::runtime_error(std::strerror(errno));
            return true;
        }
        this->_buffer.erase(0, sentBytes);
    }
    return false;
}

