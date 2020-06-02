#include "IRCServer.class.hpp"
#include "IRCMessage.class.hpp"
#include "SocketServeur.class.hpp"
#include "SocketManager.class.hpp"
#include "MessageMediator.class.hpp"
#include "ClientManager.class.hpp"
#include "ReplyManager.class.hpp"
#include "ChannelManager.class.hpp"
#include "ServerClient.class.hpp"
#include "Socket.class.hpp"
#include "Observer.class.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <string>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h>

SocketManager *IRCServer::_socket_manager = new SocketManager();
MessageMediator *IRCServer::_message_mediator = new MessageMediator();
ClientManager *IRCServer::_client_manager = new ClientManager();
ReplyManager *IRCServer::_reply_manager = new ReplyManager;
ChannelManager *IRCServer::_channel_manager = new ChannelManager();
Observer *IRCServer::_observer = new Observer();

std::string IRCServer::name;
std::string IRCServer::info;
IRCServer *IRCServer::_myself = NULL;
std::string IRCServer::_password = std::string("default");
std::vector<SocketClient *> IRCServer::_newSocketConnections = {};
std::unordered_map<Token, ServerClient *> IRCServer::_servers_local;
std::unordered_map<std::string, Token> IRCServer::_user_to_server;

IRCServer::IRCServer(void)
{
    IRCServer::_myself = this;
    this->info = "Hey I am your server";
}

IRCServer::~IRCServer(void)
{
}

void IRCServer::config(unsigned short const port, std::string const password)
{
    IRCServer::_password = password;
    SocketServeur *server = new SocketServeur(port);
    sockaddr_in std_addr = {};
    Socket *std_in = new Socket(STDIN_FILENO, *(reinterpret_cast<sockaddr *>(&std_addr)));
    IRCServer::_socket_manager->addSocket(server);
    IRCServer::_socket_manager->addSocket(std_in);
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    std::string addr;

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && (ifa->ifa_addr->sa_family == AF_INET) && !(ifa->ifa_flags & (IFF_LOOPBACK)))
        {
            sa = (struct sockaddr_in *)ifa->ifa_addr;
            addr = std::string(inet_ntoa(sa->sin_addr)) + "." + std::string(std::to_string(port));
            break;
        }
    }
    freeifaddrs(ifap);
    this->name = addr;
}

void IRCServer::connectNetwork(std::string const hostNetowrk, std::string const portNetwork)
{
    int status, sockfd;
    struct addrinfo hints = {};
    struct addrinfo *serverinfo, *ptr; // list of results

    hints.ai_family = PF_UNSPEC; //checking for both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(hostNetowrk.c_str(), portNetwork.c_str(), &hints, &serverinfo)) != 0)
        throw std::runtime_error(gai_strerror(status));
    for (ptr = serverinfo; ptr != NULL; ptr = ptr->ai_next) //on parcourt la liste des resultats possible à cette addresse
    {
        // ptr->ai_family can be IPv4 (AF_INET) or IPv6 (AF_INET6)
        if ((sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            std::cerr << "Error : socket creation for conneciton to distant server" << std::endl;
            throw std::runtime_error(std::strerror(errno));
            continue;
        }
        // ptr->ai_addr is of type sockaddr * which can be ipv4 (sockaddr_in) or v6 as well
        if (connect(sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            close(sockfd);
            throw std::runtime_error(std::strerror(errno));
            continue;
        }
        SocketClient *networkSocket = new SocketClient(sockfd, *(ptr->ai_addr));
        IRCServer::_socket_manager->addSocket(networkSocket);
        IRCServer::_newSocketConnections.push_back(networkSocket);
        std::cout << "server connecting to " << networkSocket->getAddr() << ":" << networkSocket->getPort() << std::endl;
    }
    // all done with the list of results -> can free it
    freeaddrinfo(serverinfo);
}

IRCMessage IRCServer::buildNickMessage(std::string const &nickname, unsigned int const hops, std::string const &username, std::string const &hostname, unsigned int const token, unsigned int const mode, std::string const &realname)
{
    std::vector<std::string> parameters;

    parameters.push_back(nickname);
    parameters.push_back(std::to_string(hops));
    parameters.push_back(username);
    parameters.push_back(hostname);
    parameters.push_back(std::to_string(token));
    parameters.push_back(std::to_string(mode));

    IRCMessage userMessage;
    userMessage.setCommand("NICK").setParameters(parameters).setTrail(realname, IRCMessage::IRCMessageWay::SENDING);
    return userMessage;
}

void IRCServer::joinIRCNetwork()
{
    Parameters param(*this);
    IRCMessage passMessage(param, "PASS");
    IRCMessage serverMessage(param, "SERVER"); // voir comment gerer token et hopcount

    // envoi via le MessageMediator
    for (auto it = IRCServer::_newSocketConnections.begin(); it != IRCServer::_newSocketConnections.end(); ++it)
    {
        IRCServer::_message_mediator->sendReply(passMessage.to_string(), *it);
        IRCServer::_message_mediator->sendReply(serverMessage.to_string(), *it);
    }
}

void IRCServer::replyToNewConnection(SocketClient *socket)
{
    Parameters param(*(IRCServer::_myself));
    IRCMessage passMessage(param, "PASS");
    IRCMessage serverMessage(param, "SERVER");

    std::vector<SocketClient *>::iterator it;
    it = std::find(IRCServer::_newSocketConnections.begin(), IRCServer::_newSocketConnections.end(), socket);
    if (it == IRCServer::_newSocketConnections.end())
    {
        IRCServer::_message_mediator->sendReply(passMessage.to_string(), socket);
        IRCServer::_message_mediator->sendReply(serverMessage.to_string(), socket);
        IRCServer::sendDataServer(socket);
        IRCServer::sendDataUser(socket);
    }
    else
        IRCServer::_newSocketConnections.erase(it);
}

Token IRCServer::addServer(ServerClient &server)
{
    Token token = 1;
    do
    {
        token++;
    } while (IRCServer::_servers_local.find(token) != IRCServer::_servers_local.end());
    std::pair<Token, ServerClient *> value(token, &server);
    IRCServer::_servers_local.insert(value);
    return token;
}

void IRCServer::addUser(User &user, Token token)
{
    std::pair<std::string, Token> value(user.getName(), token);
    IRCServer::_user_to_server.insert(value);
    IRCServer::_observer->notify(&user, "NICK");
}

void IRCServer::sendDataServer(SocketClient *socket)
{
    for (auto i = IRCServer::_servers_local.begin(); i != IRCServer::_servers_local.end(); i++)
    {
        if (socket != i->second->getSocketClient())
        {
            Parameters params(*(i->second));
            IRCMessage serverMessage(params, "SERVER");
            IRCServer::_message_mediator->sendReply(serverMessage.to_string(), socket);
        }
    }
}

void IRCServer::sendDataUser(SocketClient *socket)
{
    std::vector<User *> users = IRCServer::_client_manager->getUsers();
    if (!users.empty())
    {
        for (auto user_it = users.begin(); user_it != users.end(); user_it++)
        {
            User *user = (*user_it);
            if (socket != user->getSocketClient())
            {
                ServerClient *server = dynamic_cast<ServerClient *>(IRCServer::_client_manager->getClient(user->getSocketClient()));
                int hop = server ? server->getHopcount() + 1 : 1;
                Token token = server ? server->getToken() : 1;
                IRCMessage nick_message = IRCServer::buildNickMessage(user->getName(), hop, user->getUser(), IRCServer::name, token, user->getModeInt(), "ok");
                IRCServer::_message_mediator->sendReply(nick_message.to_string(), socket);
            }
        }
    }
}

ServerClient *IRCServer::getServerClient(Token token)
{
    ServerClient *server = NULL;
    try
    {
        IRCServer::_servers_local.at(token);
    }
    catch (std::out_of_range &e)
    {
        std::cout << e.what() << std::endl;
    }
    return server;
}

void IRCServer::run()
{
    std::cout << "Starting server " << this->name << ": hit return to shutdown" << std::endl;
    while (1)
    {
        if (!IRCServer::_socket_manager->route())
            break;
    }
}

void IRCServer::stop()
{
}

std::string IRCServer::getPassword(void) const
{
    return this->_password;
}

IRCServer *IRCServer::getInstance(void)
{
    return IRCServer::_myself;
}

ServerClient *IRCServer::getServerFromUser(std::string const &nickname)
{
    ServerClient *server = NULL;
    auto it = IRCServer::_user_to_server.find(nickname);
    if (it != IRCServer::_user_to_server.end() && it->second != 1)
    {
        Token token = IRCServer::_user_to_server.at(nickname);
        server = IRCServer::_servers_local.at(token);
    }
    return server;
}
