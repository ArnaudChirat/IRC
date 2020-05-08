#include "MessageMediator.class.hpp"
#include <iostream>
#include "IRCServer.class.hpp"

MessageMediator::MessageMediator(void)
{
    this->_commands.insert({IRCMessageType::NICK, &MessageMediator::createClient});
    this->_commands.insert({IRCMessageType::SERVICE, &MessageMediator::createClient});
    this->_commands.insert({IRCMessageType::USER, &MessageMediator::userCommand});
    return;
}

MessageMediator::~MessageMediator(void)
{
    return;
}

bool MessageMediator::handleMessage(IRCMessage const &message, SocketClient *socket)
{
    std::cout << "Message mediator : " << std::endl;
    std::cout << message << std::endl;
    // std::cout << (this->_commands.at(message.type)) << std::endl;
    (this->*_commands[message.type])(message, socket);
    // if ((message.type & IRCMessageType::IRCMessage_TYPE_MASK) == IRCMessageType::CONNECTION_REGISTRATION)
    //     this->createClient(message, socket);
    return (true);
}

void MessageMediator::createClient(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = NULL;
    if (message.type == IRCMessageType::NICK)
    {
        client = IRCServer::_client_manager.createAddClient(ClientManager::USER, socket, message.getParameters()[0]);
    }
    if (client)
        std::cout << "Client created :" << client->getName() << std::endl;
    else
        std::cout << "Client already exist" << std::endl;
}

void MessageMediator::userCommand(IRCMessage const &message, SocketClient *socket) const
{
    if (!IRCServer::_client_manager.setUser(message.getParameters()[0], socket))
        std::cout << "Nick not set or socket doesnt exist" << std::endl;
    else
        std::cout << "Client Connected" << std::endl;
}