#ifndef IRCSERVER_CLASS_HPP
#define IRCSERVER_CLASS_HPP
#include "SocketManager.class.hpp"
#include "MessageMediator.class.hpp"
#include "ClientManager.class.hpp"
class IRCServer
{
public:
    IRCServer(void);
    // IRCServer(IRCServer const &instance);
    // IRCServer &operator=(IRCServer const &rhs);
    ~IRCServer(void);

    void run();
    void stop();
    static SocketManager _socket_manager;
    static MessageMediator _message_mediator;
    static ClientManager _client_manager;
};

#endif