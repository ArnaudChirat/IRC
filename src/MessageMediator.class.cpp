#include "MessageMediator.class.hpp"
MessageMediator::~MessageMediator(void)
{
    return;
}

MessageMediator::~MessageMediator(void)
{
    return;
}

bool MessageMediator::handleMessage(IRCMessage const &message) const
{
    if (message.type & IRCMessageType::IRCMessage_TYPE_MASK == IRCMessageType::CONNECTION_REGISTRATION)
        this->createClient(message);
    return (true);
}

void MessageMediator::createClient(IRCMessage const &message) const
{

}