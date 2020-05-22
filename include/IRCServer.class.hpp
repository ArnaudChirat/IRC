#ifndef IRCSERVER_CLASS_HPP
#define IRCSERVER_CLASS_HPP

#include <string>
#include <vector>
#include <map>
#include "Utility.hpp"
#include "ServerClient.class.hpp"

class SocketManager;
class SocketClient;
class MessageMediator;
class ClientManager;
class ReplyManager;
class ChannelManager;
class IRCMessage;

class IRCServer
{
private:
    static std::string   _password;
    static std::vector<SocketClient *>     _newSocketConnections;
    static std::map<Token, ServerClient*> _servers_local;

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
    void  joinIRCNetwork(void);
    static void  replyToNewConnection(unsigned int const & hops, SocketClient * socket);
    static bool checkToken(Token token);
    static void addServer(Token token, ServerClient &server);
    static void sendServerNeighborData(ServerClient &server);
    static IRCMessage  buildPassMessage(void);
    static IRCMessage  buildServerMessage(std::string const & newServer, unsigned int const &hops, unsigned int const &token, std::string const &info);
    static SocketManager * _socket_manager;
    static MessageMediator * _message_mediator;
    static ClientManager * _client_manager;
    static ReplyManager * _reply_manager;
    static ChannelManager * _channel_manager;
};

#endif