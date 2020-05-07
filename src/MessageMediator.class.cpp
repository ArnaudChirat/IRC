#include "MessageMediator.class.hpp"
#include <iostream>
#include "IRCServer.class.hpp"
MessageMediator::MessageMediator(void)
{
    return;
}

MessageMediator::~MessageMediator(void)
{
    return;
}

bool MessageMediator::handleMessage(IRCMessage const &message, SocketClient *socket) const
{
    std::cout << message << std::endl;
    if ((message.type & IRCMessageType::IRCMessage_TYPE_MASK) == IRCMessageType::CONNECTION_REGISTRATION)
        this->createClient(message, socket);
    return (true);
}

void MessageMediator::createClient(IRCMessage const &message, SocketClient *socket) const
{
    if (message.type == IRCMessageType::NICK)
    {
        Client *client = IRCServer::_client_manager.createAddClient(ClientManager::USER, socket, message.getParameters()[0]);
        if (client)
            std::cout << "Client created :" << client->getName() << std::endl;
        else
            std::cout << "Client already exist" << std::endl;
        
    }
}