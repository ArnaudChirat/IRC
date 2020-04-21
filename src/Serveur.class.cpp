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

Serveur::Serveur(void) : _sockets()
{
    this->_serveur_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->_serveur_socket < 0)
        throw std::runtime_error(std::strerror(errno));
}

Serveur::Serveur(Serveur const &instance) : _sockets(instance._sockets)
{
    this->_serveur_socket = Utility::safe_dup(instance._serveur_socket);
}

Serveur &Serveur::operator=(Serveur const &rhs)
{
    if (this != &rhs)
    {
        if (this->_serveur_socket < 0)
            close(this->_serveur_socket);
        this->_serveur_socket = Utility::safe_dup(rhs._serveur_socket);
        this->_sockets = rhs._sockets;
    }
    return (*this);
}

Serveur::~Serveur(void)
{
    if (!_sockets.empty())
        _sockets.clear();
    std::cout << _serveur_socket << std::endl;
}

void Serveur::acceptNewClient()
{
    sockaddr_in addr = {};
    socklen_t len = sizeof(addr);
    int newClient = accept(this->_serveur_socket, reinterpret_cast<sockaddr *>(&addr), &len);
    if (newClient != -1)
    {
        Socket newSocket;
        newSocket.sckt = newClient;
        newSocket.addr = addr;
        std::cout << "Connexion de #" << newSocket.sckt << " from " << this->getAddress(addr).c_str() << ":" << ntohs(addr.sin_port) << std::endl;
        this->_sockets.push_front(newSocket);
    }
}

void Serveur::setFdSet()
{
    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_ZERO(&_errorfds);
    _max_fd = 0;
    for (auto &Socket : this->_sockets)
    {
        FD_SET(Socket.sckt, &_readfds);
        FD_SET(Socket.sckt, &_writefds);
        FD_SET(Socket.sckt, &_errorfds);
        if (Socket.sckt > _max_fd)
            _max_fd = Socket.sckt;
    }
}

void Serveur::createServeur(unsigned short port)
{
    sockaddr_in server;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (bind(this->_serveur_socket, reinterpret_cast<sockaddr *>(&server), sizeof(server)) == -1)
        throw std::runtime_error(std::strerror(errno));
    if (listen(this->_serveur_socket, SOMAXCONN) == -1)
        throw std::runtime_error(std::strerror(errno));
    Socket newSocket;
    newSocket.sckt = this->_serveur_socket;
    newSocket.addr = server;
    this->_sockets.push_front(newSocket);
    std::cout << "Serveur demarre sur le port : " << port << std::endl;
}

bool Serveur::clientSocket(const std::string &socket_address, const int socket_port, std::forward_list<Socket>::iterator it) const
{
    char buffer[200] = {0};
    int ret = recv(it->sckt, buffer, 199, 0);
    if (ret == 0)
    {
        std::cout << "Connexion terminee" << std::endl;
        return (true);
    }
    else if (ret == -1)
    {
        std::cout << "Erreur reception : " << std::strerror(errno) << std::endl;
        return (true);
    }
    else
    {
        std::cout << "[" << socket_address << ":" << socket_port << "] " << buffer;
        // if (FD_ISSET(it->sckt, &writefds))
        // {
        //     ret = send(it->sckt, buffer, ret, 0);
        //     if (ret == 0 || ret == -1)
        //     {
        //         std::cout << "Erreur envoi" << std::strerror(errno) << std::endl;
        //         hasError = true;
        //     }
        // }
    }
    return (false);
}

void Serveur::launchServeur(unsigned short port)
{
    this->createServeur(port);
    while (1)
    {
        this->setFdSet();
        int selectRes = select(_max_fd + 1, &_readfds, &_writefds, &_errorfds, NULL);
        if (selectRes == -1)
            throw std::runtime_error(std::strerror(errno));
        else if (selectRes > 0)
        {
            auto it = this->_sockets.begin();
            auto it_before = this->_sockets.before_begin();
            while (it != this->_sockets.end())
            {
                const std::string socket_address = Serveur::getAddress(it->addr);
                const unsigned short socket_port = ntohs(it->addr.sin_port);

                bool has_error = false;
                if (FD_ISSET(it->sckt, &_errorfds))
                {
                    std::cout << "Erreur" << std::endl;
                    has_error = true;
                }
                else if (FD_ISSET(it->sckt, &_readfds))
                {
                    if (it->sckt != this->_serveur_socket)
                        has_error = this->clientSocket(socket_address, socket_port, it);
                    else
                        this->acceptNewClient();
                }
                if (has_error)
                {
                    std::cout << "Deconnexion de [" << socket_address << ":" << socket_port << "]" << std::endl;
                    this->_sockets.erase_after(it_before);
                    it = it_before;
                    it++;
                }
                else
                {
                    it_before = it;
                    ++it;
                }
            }
        }
    }
};

std::string Serveur::getAddress(const sockaddr_in &addr) const
{
    char buff[INET6_ADDRSTRLEN];
    return inet_ntop(addr.sin_family, (void *)&(addr.sin_addr), buff, INET6_ADDRSTRLEN);
};