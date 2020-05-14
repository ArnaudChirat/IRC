#include "IRCServer.class.hpp"
#include "SocketServeur.class.hpp"
#include "SocketManager.class.hpp"
#include "MessageMediator.class.hpp"
#include "ClientManager.class.hpp"
#include "ReplyManager.class.hpp"
#include "ChannelManager.class.hpp"
#include <unistd.h>
#include <iostream>

SocketManager * IRCServer::_socket_manager = new SocketManager();
MessageMediator * IRCServer::_message_mediator = new MessageMediator();
ClientManager * IRCServer::_client_manager = new ClientManager();
ReplyManager * IRCServer::_reply_manager = new ReplyManager; 
ChannelManager * IRCServer::_channel_manager = new ChannelManager();

std::string const IRCServer::name = std::string("GvannAchir IRC");

IRCServer::IRCServer(void)
{

    SocketServeur *server = new SocketServeur(6667);
    sockaddr_in std_addr = {};
    Socket *std_in = new Socket(STDIN_FILENO, std_addr);
    IRCServer::_socket_manager->addSocket(server);
    IRCServer::_socket_manager->addSocket(std_in);
}

IRCServer::~IRCServer(void)
{
}

void IRCServer::run()
{
    std::cout << "Starting server: hit return to shutdown" << std::endl;
    while (1)
    {
        if (!IRCServer::_socket_manager->route())
            break;;
    }
}

void IRCServer::stop()
{
}
