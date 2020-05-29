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

std::string IRCServer::name;
std::string IRCServer::_password = std::string("default");
std::vector<SocketClient *> IRCServer::_newSocketConnections = {};
std::map<Token, ServerClient *> IRCServer::_servers_local;

IRCServer::IRCServer(void)
{
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
            addr = std::string(inet_ntoa(sa->sin_addr)) + ".." + std::string(std::to_string(port));
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
    freeaddrinfo(serverinfo); // all done with the list of results -> can free it
}

IRCMessage IRCServer::buildPassMessage(void)
{
    std::string prefix = IRCServer::name;
    std::vector<std::string> parameters;
    parameters.push_back(IRCServer::_password);
    IRCMessage passMessage;
    passMessage.setPrefix(prefix, IRCMessage::IRCMessageWay::SENDING).setCommand("PASS").setParameters(parameters);
    return passMessage;
}

IRCMessage IRCServer::buildServerMessage(std::string const &newServer, unsigned int const &hops, unsigned int const &token, std::string const &info)
{
    std::string prefix = IRCServer::name;

    std::vector<std::string> parameters;
    parameters.push_back(newServer);
    parameters.push_back(std::to_string(hops));
    parameters.push_back(std::to_string(token));

    std::string trail = info;
    IRCMessage serverMessage;
    serverMessage.setPrefix(prefix, IRCMessage::IRCMessageWay::SENDING).setCommand("SERVER").setParameters(parameters).setTrail(trail, IRCMessage::IRCMessageWay::SENDING);
    return serverMessage;
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
    // creation IRCMessage password => a voir pour rajouter les infos
    IRCMessage passMessage = IRCServer::buildPassMessage();
    IRCMessage serverMessage = IRCServer::buildServerMessage(IRCServer::name, 1, 1, "Salut BG");

    // envoi via le MessageMediator
    for (auto it = IRCServer::_newSocketConnections.begin(); it != IRCServer::_newSocketConnections.end(); ++it)
    {
        passMessage.setSocket(*it);
        serverMessage.setSocket(*it);
        IRCServer::_message_mediator->sendReply(passMessage);
        IRCServer::_message_mediator->sendReply(serverMessage);
    }
}

void IRCServer::replyToNewConnection(unsigned int const &hops, SocketClient *socket, Token token)
{
    std::vector<SocketClient *>::iterator it;
    it = std::find(IRCServer::_newSocketConnections.begin(), IRCServer::_newSocketConnections.end(), socket);
    if (it == IRCServer::_newSocketConnections.end())
    {
        IRCMessage passMessage = IRCServer::buildPassMessage();
        // quel token en reponse au server?? Meme hops que le server, mais quelle token?
        IRCMessage serverMessage = IRCServer::buildServerMessage(IRCServer::name, hops, token, "Main Server");
        passMessage.setSocket(socket);
        serverMessage.setSocket(socket);
        IRCServer::_message_mediator->sendReply(passMessage);
        IRCServer::_message_mediator->sendReply(serverMessage);
        IRCServer::sendDataServer(socket);
        IRCServer::sendDataUser(socket);
    }
    else
        IRCServer::_newSocketConnections.erase(it);
}

void IRCServer::addServer(ServerClient &server)
{
    Token token = 1;
    do
    {
        token++;
    } while (IRCServer::_servers_local.find(token) != IRCServer::_servers_local.end());
    std::pair<Token, ServerClient *> value(token, &server);
    IRCServer::_servers_local.insert(value);
}

void IRCServer::sendDataServer(SocketClient *socket)
{
    for (auto i = IRCServer::_servers_local.begin(); i != IRCServer::_servers_local.end(); i++)
    {
        if (socket != i->second->getSocketClient())
        {
            IRCMessage serverMessage = IRCServer::buildServerMessage(i->second->getName(), i->second->getHopcount() + 1, i->first, "Other servers");
            serverMessage.setSocket(socket);
            IRCServer::_message_mediator->sendReply(serverMessage);
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
                nick_message.setSocket(socket);
                IRCServer::_message_mediator->sendReply(nick_message);
            }
        }
    }
}

ServerClient *IRCServer::getServerClient(Token token)
{
    auto it = IRCServer::_servers_local.find(token);
    if (it != IRCServer::_servers_local.end())
        return (it->second);
    return (NULL);
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
