#include "SocketClient.class.hpp"
#include "IRCMessage.class.hpp"
#include "MessageMediator.class.hpp"
#include <iostream>
#include "Utility.hpp"
#include <time.h>
#include <cstring>
#include <regex>
#include "IRCServer.class.hpp"
SocketClient::SocketClient(void)
{
    return;
}

SocketClient::SocketClient(int const socket, sockaddr const & addr) : Socket(socket, addr)
{
    return;
}

SocketClient::~SocketClient(void)
{
}

void    SocketClient::addToQueue(std::string const & msg){
    this->_msgQueue.push(msg);
}


bool SocketClient::recvMessage() {
    int ret = 0;
    char buffer[BUFF_MSG] = {0};
    int sckt = this->getSocket();
    ret = recv(sckt, buffer, BUFF_MSG - 1, 0);
    buffer[BUFF_MSG - 1] = '\0';
    if (ret == 0)
    {
        std::cout << "Connexion terminee" << std::endl;
        IRCServer::_client_manager->deleteClient(this, ClientManager::ClientChoice::USER);
        return (true);
    }
    else if (ret == -1)
    {
        std::cout << "Erreur reception : " << std::strerror(errno) << std::endl;
        return (true);
    }
    else if (ret > 1)
    {
        this->_buffer += buffer;
        if (this->_buffer.find('\n') != std::string::npos){
            std::cout << "[" << this->getAddr().c_str() << ":" << this->getPort() << "] " << this->_buffer;
            std::vector<std::string>  messages = Utility::splitParam(this->_buffer, "\n");
            char c = this->_buffer.back();
            this->_buffer.clear();
            std::cout << messages[0] << std::endl;
            auto it = messages.rbegin();
            if (c != '\n'){
                this->_buffer = *it;
                messages.pop_back();
            }
            for (auto it = messages.begin(); it != messages.end(); ++it){
                IRCMessage IRC_message(*it, this);
                if (IRC_message.isCommand(this))
                    IRCServer::_message_mediator->handleMessage(IRC_message, this);
            }
        }
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
    if (!this->_msgQueue.empty()){
        std::string buffer = this->_msgQueue.front();
        while (buffer.size()){
            if ((sentBytes = send(this->getSocket(), buffer.c_str(), buffer.size(), 0)) == -1){
                throw std::runtime_error(std::strerror(errno));
                return true;
            }
            buffer.erase(0, sentBytes);
        }
        this->_msgQueue.pop();
    }
    return false;
}

std::string     SocketClient::getPassword(void) const{
    return this->_password;
}

void    SocketClient::setPassword(std::string const & password){
    this->_password = password;
}