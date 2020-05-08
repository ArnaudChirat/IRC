#include "IRCServer.class.hpp"
#include <unistd.h>
#include <iostream>

SocketManager IRCServer::_socket_manager = SocketManager();
MessageMediator IRCServer::_message_mediator = MessageMediator();
ClientManager IRCServer::_client_manager = ClientManager();
ReplyManager IRCServer::_reply_manager = ReplyManager();

std::string const IRCServer::name = std::string("GvannAchir IRC");

IRCServer::IRCServer(void)
{

    SocketServeur *server = new SocketServeur(6667);
    sockaddr_in std_addr = {};
    Socket *std_in = new Socket(STDIN_FILENO, std_addr);
    IRCServer::_socket_manager.addSocket(server);
    IRCServer::_socket_manager.addSocket(std_in);
}

IRCServer::~IRCServer(void)
{
}

void IRCServer::run()
{
    std::cout << "Starting server: hit return to shutdown" << std::endl;
    while (1)
    {
        if (!IRCServer::_socket_manager.route())
            break;;
    }
}

void IRCServer::stop()
{
}
