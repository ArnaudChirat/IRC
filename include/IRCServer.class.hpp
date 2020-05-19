#ifndef IRCSERVER_CLASS_HPP
#define IRCSERVER_CLASS_HPP

#include <string>

class SocketManager;
class MessageMediator;
class ClientManager;
class ReplyManager;
class ChannelManager;

class IRCServer
{
private:
    std::string   _password;

public:
    IRCServer(void);
    static std::string const name;
    IRCServer(IRCServer const &instance) = delete;
    IRCServer &operator=(IRCServer const &rhs) = delete;
    ~IRCServer(void);


    void run();
    void stop();
    void config(unsigned short const port, std::string const password);
    void connectNetwork(std::string const hostNetowrk, std::string const portNetwork);
    static SocketManager * _socket_manager;
    static MessageMediator * _message_mediator;
    static ClientManager * _client_manager;
    static ReplyManager * _reply_manager;
    static ChannelManager * _channel_manager;
};

#endif