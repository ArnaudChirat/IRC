#ifndef IRCSERVER_CLASS_HPP
#define IRCSERVER_CLASS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "Utility.hpp"

class SocketManager;
class SocketClient;
class ServerClient;
class MessageMediator;
class ClientManager;
class ReplyManager;
class ChannelManager;
class IRCMessage;
class Observer;

class IRCServer
{
private:
    static std::string _password;
    static IRCServer * _myself;
    static std::vector<SocketClient *> _newSocketConnections;
    static std::unordered_map<Token, ServerClient *> _servers_local;
    static std::unordered_map<std::string, Token> _user_to_server;
public:
    IRCServer(void);
    static std::string name;
    static std::string info;
    IRCServer(IRCServer const &instance) = delete;
    IRCServer &operator=(IRCServer const &rhs) = delete;
    ~IRCServer(void);

    void run();
    void stop();
    void config(unsigned short const port, std::string const password);
    void connectNetwork(std::string const hostNetowrk, std::string const portNetwork);
    void joinIRCNetwork(void);
    static IRCServer * getInstance(void);
    static ServerClient * getServerFromUser(std::string const &);
    static void replyToNewConnection(SocketClient *socket);
    static void sendDataServer(SocketClient *socket);
    static void sendDataUser(SocketClient *socket);
    static void replyToNewConnection(unsigned int const &hops, SocketClient *socket, Token token);
    static Token addServer(ServerClient &server);
    static void addUser(User &user, Token token);


    static ServerClient *getServerClient(Token token);
    static IRCMessage buildPassMessage(void);
    static IRCMessage buildNickMessage(std::string const &nickname, unsigned int const hops, std::string const &username, std::string const &hostname ,unsigned int const token, unsigned int const mode, std::string const &realname);
    static IRCMessage buildServerMessage(std::string const &newServer, unsigned int const &hops, unsigned int const &token, std::string const &info);
    static SocketManager *_socket_manager;
    static MessageMediator *_message_mediator;
    static ClientManager *_client_manager;
    static ReplyManager *_reply_manager;
    static ChannelManager *_channel_manager;
    static Observer *_observer;

    std::string     getPassword(void) const;
};

#endif