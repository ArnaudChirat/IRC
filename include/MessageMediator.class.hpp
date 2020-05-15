#ifndef MESSAGEMEDIATOR_CLASS_HPP
#define MESSAGEMEDIATOR_CLASS_HPP
#include "IRCMessage.class.hpp"
#include "SocketClient.class.hpp"
#include <map>

class MessageMediator
{
public:
    MessageMediator(void);
    // MessageMediator(MessageMediator const &instance);
    // MessageMediator &operator=(MessageMediator const &rhs);
    ~MessageMediator(void);
    bool handleMessage(IRCMessage const &message, SocketClient *client);
    bool sendReply(std::string const &, SocketClient *) const;

private:
    typedef void (MessageMediator::*Command)(IRCMessage const &, SocketClient *) const;
    void userCommand(IRCMessage const &message, SocketClient *socket) const;
    void createClient(IRCMessage const &message, SocketClient *socket) const;
    void quitCommand(IRCMessage const &message, SocketClient *socket) const;
    void joinCommand(IRCMessage const &message, SocketClient *socket) const;
    void passCommand(IRCMessage const &message, SocketClient *socket) const;
    void operCommand(IRCMessage const &message, SocketClient *socket) const;
    void partCommand(IRCMessage const &message, SocketClient *socket) const;
    std::map<IRCMessage::IRCMessageType, Command> _commands;
};

#endif