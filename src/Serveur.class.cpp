#include "Serveur.class.hpp"
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

Serveur::Serveur(void)
{
    return;
}

Serveur::Serveur(unsigned short port)
{
    int sck = this->getSocket();
    sockaddr_in server;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    this->setAddr(server);
    if (bind(sck, reinterpret_cast<sockaddr *>(&server), sizeof(server)) == -1)
        throw std::runtime_error(std::strerror(errno));
    if (listen(sck, SOMAXCONN) == -1)
        throw std::runtime_error(std::strerror(errno));
}

Serveur::~Serveur(void)
{
  return ;
}

Client *Serveur::acceptNewClient()
{
    sockaddr_in addr = {};
    socklen_t len = sizeof(addr);
    int sck = this->getSocket();
    int newSocket = accept(sck, reinterpret_cast<sockaddr *>(&addr), &len);
    if (newSocket != -1)
    {
        Client *newClient = new Client(newSocket, addr);
        std::cout << "Connexion de #" << newClient->getSocket() << " from " << newClient->getAddr().c_str() << ":" << newClient->getPort() << std::endl;
        send(newSocket, "Tu es connecté\n", 16, 0);
        return newClient;
    }
    return (NULL);
}


void Serveur::handle(SocketManagerInterface &dispatcher)
{
    dispatcher.dispatch(*this);
}