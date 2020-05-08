#ifndef IRCSERVER_CLASS_HPP
#define IRCSERVER_CLASS_HPP
#include "SocketManager.class.hpp"
#include "MessageMediator.class.hpp"
#include "ClientManager.class.hpp"
#include "ReplyManager.class.hpp"
class IRCServer
{
public:
    static std::string const name;
    IRCServer(void);
    IRCServer(IRCServer const &instance) = delete;
    IRCServer &operator=(IRCServer const &rhs) = delete;
    ~IRCServer(void);


    void run();
    void stop();
    static SocketManager _socket_manager;
    static MessageMediator _message_mediator;
    static ClientManager _client_manager;
    static ReplyManager _reply_manager;
};

#endif