#include "SocketManager.class.hpp"
#include "Serveur.class.hpp"
#include "Client.class.hpp"
#include <sys/select.h>
#include <cerrno>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
SocketManager::SocketManager(void)
{
    return;
}

SocketManager::SocketManager(Socket *serveur)
{
    sockaddr_in  std_addr;
    Socket *std_in = new Socket(STDIN_FILENO, std_addr);
    this->addSocket(std_in);
    this->addSocket(serveur);
    return;
}

SocketManager::~SocketManager(void)
{
}

void SocketManager::addSocket(Socket *socket)
{
    this->_sockets.emplace_back(socket);
}

void SocketManager::setFdSet()
{
    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_ZERO(&_errorfds);
    _max_fd = 0;
    for (auto &socket : this->_sockets)
    {
        int sckt = socket->getSocket();
        FD_SET(sckt, &_readfds);
        FD_SET(sckt, &_writefds);
        FD_SET(sckt, &_errorfds);
        if (sckt > _max_fd)
            _max_fd = sckt;
    }
}

void SocketManager::route()
{
    while (1)
    {
        this->setFdSet();
        int selectRes = select(_max_fd + 1, &_readfds, &_writefds, &_errorfds, NULL);
        if (selectRes == -1)
            throw std::runtime_error(std::strerror(errno));
        else if (selectRes > 0)
        {
            auto it = this->_sockets.begin();
            while (it != this->_sockets.end())
            {
                const std::string socket_address = (*it)->getAddr();
                const unsigned short socket_port = ntohs((*it)->getPort());
                const int sckt = (*it)->getSocket();
                bool has_error = false;
                if (FD_ISSET(sckt, &_errorfds))
                {
                    std::cout << "Erreur" << std::endl;
                    has_error = true;
                }
                else if (FD_ISSET(sckt, &_readfds))
                {
                    if (Serveur *serveur = dynamic_cast<Serveur *>(*it))
                        this->addSocket(serveur->acceptNewClient());
                    if (Client *client = dynamic_cast<Client *>(*it))
                        has_error = client->recvMessage();
                    if (sckt == STDIN_FILENO) {
                        std::string in;
                        std::getline(std::cin, in);
                        std::cout << in << std::endl;
                    }
                }
                if (has_error)
                {
                    std::cout << "Deconnexion de [" << socket_address << ":" << socket_port << "]" << std::endl;
                    this->_sockets.erase(it);
                }
                ++it;
            }
        }
    }
}