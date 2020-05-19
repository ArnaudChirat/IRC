#include "IRCServer.class.hpp"
#include "SocketServeur.class.hpp"
#include "SocketManager.class.hpp"
#include "MessageMediator.class.hpp"
#include "ClientManager.class.hpp"
#include "ReplyManager.class.hpp"
#include "ChannelManager.class.hpp"
#include "Socket.class.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <cstring>

SocketManager * IRCServer::_socket_manager = new SocketManager();
MessageMediator * IRCServer::_message_mediator = new MessageMediator();
ClientManager * IRCServer::_client_manager = new ClientManager();
ReplyManager * IRCServer::_reply_manager = new ReplyManager; 
ChannelManager * IRCServer::_channel_manager = new ChannelManager();

std::string const IRCServer::name = std::string("GvannAchir IRC");

IRCServer::IRCServer(void) {}

IRCServer::~IRCServer(void)
{
}

void IRCServer::config(unsigned short const port, std::string const password){
    this->_password = password;
    SocketServeur *server = new SocketServeur(port);
    sockaddr_in std_addr = {};
    Socket *std_in = new Socket(STDIN_FILENO, *(reinterpret_cast<sockaddr *>(&std_addr)));
    IRCServer::_socket_manager->addSocket(server);
    IRCServer::_socket_manager->addSocket(std_in);
}

void  IRCServer::connectNetwork(std::string const hostNetowrk, std::string const portNetwork){
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
        if ((sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1){
            std::cerr << "Error : socket creation for conneciton to distant server" << std::endl;
            throw std::runtime_error(std::strerror(errno));
            continue;
        }
        // ptr->ai_addr is of type sockaddr * which can be ipv4 (sockaddr_in) or v6 as well
        if (connect(sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1){
            close(sockfd);
            throw std::runtime_error(std::strerror(errno));
            continue;
        }
        SocketClient * networkSocket = new SocketClient(sockfd, *(ptr->ai_addr));
        IRCServer::_socket_manager->addSocket(networkSocket);
        std::cout << "server connecting to " << networkSocket->getAddr() << ":" << networkSocket->getPort() << std::endl;
        // IRCServer::_message_mediator->sendReply("coucou je me connecte à toi!", networkSocket);
    }
    freeaddrinfo(serverinfo); // all done with the list of results -> can free it
}

void IRCServer::run()
{
    std::cout << "Starting server: hit return to shutdown" << std::endl;
    while (1)
    {
        if (!IRCServer::_socket_manager->route())
            break;
    }
}



void IRCServer::stop()
{
}
