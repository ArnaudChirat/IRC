#ifndef MESSAGEMEDIATOR_CLASS_HPP
#define MESSAGEMEDIATOR_CLASS_HPP
#include "IRCMessage.class.hpp"
#include "SocketClient.class.hpp"
class MessageMediator
{
public:
    MessageMediator(void);
    // MessageMediator(MessageMediator const &instance);
    // MessageMediator &operator=(MessageMediator const &rhs);
    ~MessageMediator(void);
    bool handleMessage(IRCMessage const &message, SocketClient *client) const;

private:
    void createClient(IRCMessage const &message, SocketClient *socket) const;
};

#endif