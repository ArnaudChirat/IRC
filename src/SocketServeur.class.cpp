#include "SocketServeur.class.hpp"
#include "Utility.hpp"
#include <stdexcept>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

SocketServeur::SocketServeur(void)
{
    return;
}

SocketServeur::SocketServeur(unsigned short port)
{
    int sck = this->getSocket();
    int option = 1;
    sockaddr_in server;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    this->setAddr(server);
    if (bind(sck, reinterpret_cast<sockaddr *>(&server), sizeof(server)) == -1)
        throw std::runtime_error(std::strerror(errno));
    if (listen(sck, SOMAXCONN) == -1)
        throw std::runtime_error(std::strerror(errno));
    if (setsockopt(sck, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
        throw std::runtime_error(std::strerror(errno));
}   

SocketServeur::~SocketServeur(void)
{
  return ;
}

SocketClient *SocketServeur::acceptNewClient()
{
    sockaddr_in addr = {};
    socklen_t len = sizeof(addr);
    int sck = this->getSocket();
    int newSocket = accept(sck, reinterpret_cast<sockaddr *>(&addr), &len);
    if (newSocket != -1)
    {
        SocketClient *newClient = new SocketClient(newSocket, addr);
        std::cout << "Acceptation de #" << newClient->getSocket() << " from " << newClient->getAddr().c_str() << ":" << newClient->getPort() << std::endl;
        // send(newSocket, "Tu es connecté\n", 16, 0);
        return newClient;
    }
    return (NULL);
}


void SocketServeur::handle(SocketManagerInterface &dispatcher)
{
    dispatcher.dispatch(*this);
}