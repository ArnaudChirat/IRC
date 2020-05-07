#include "IRCServer.class.hpp"
#include <unistd.h>
#include <iostream>
SocketManager IRCServer::_socket_manager = SocketManager();
MessageMediator IRCServer::_message_mediator = MessageMediator();
ClientManager IRCServer::_client_manager = ClientManager();

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
    while (1)
    {
        IRCServer::_socket_manager.route();
    }
}

void IRCServer::stop()
{
}
