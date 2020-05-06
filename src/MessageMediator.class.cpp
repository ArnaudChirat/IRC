#include "MessageMediator.class.hpp"
#include <iostream>
#include "ClientFactory.class.hpp"
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
        Client *new_client = ClientFactory::createClient(ClientFactory::USER, socket);
        new_client->setName(message.getParameters()[0]);
        std::cout << new_client->getName() << std::endl;
    }
}