#ifndef MESSAGEMEDIATOR_CLASS_HPP
#define MESSAGEMEDIATOR_CLASS_HPP

#include "Utility.hpp"

#include "IRCMessage.class.hpp"
#include "SocketClient.class.hpp"
#include <map>
#include <functional>

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
    void njoinCommand(IRCMessage const &message, SocketClient *socket) const;
    void passCommand(IRCMessage const &message, SocketClient *socket) const;
    void operCommand(IRCMessage const &message, SocketClient *socket) const;
    void partCommand(IRCMessage const &message, SocketClient *socket) const;
    void privmsgCommand(IRCMessage const &message, SocketClient *socket) const;
    void serverCommand(IRCMessage const &message, SocketClient *socket) const;
    void lusersCommand(IRCMessage const &message, SocketClient *socket) const;
    void squitCommand(IRCMessage const &message, SocketClient *socket) const;
    void nickServerCommand(IRCMessage const &message, SocketClient *socket) const;
    std::map<IRCMessage::IRCMessageType, Command> _commands;
};

#endif