#include "SocketManager.class.hpp"
#include "Socket.class.hpp"
#include "Observer.class.hpp"
#include "IRCServer.class.hpp"
#include <sys/select.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
SocketManager::SocketManager(void)
{
    return;
}

// SocketManager::SocketManager(SocketServeur &serveur, Socket &std_in)
// {
//     // sockaddr_in std_addr;
//     // Socket *std_in = new Socket(STDIN_FILENO, std_addr);
//     this->addSocket(&std_in);
//     this->addSocket(&serveur);
//     return;
// }


SocketManager::~SocketManager(void)
{
}

void SocketManager::addSocket(Socket *socket)
{
    this->_sockets.emplace_back(socket);
}

void SocketManager::deleteSocket(Socket *socket)
{
    SocketManager::pos it = this->_sockets.begin();
    for (; it != this->_sockets.end(); ++it)
    {
        Socket *tmp_socket = it->get();
        if (tmp_socket == socket)
            this->_sockets.erase(it);
    }
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

bool SocketManager::route()
{
    this->setFdSet();
    int selectRes = select(_max_fd + 1, &_readfds, &_writefds, &_errorfds, NULL);
    if (selectRes == -1)
        throw std::runtime_error(std::strerror(errno));
    else if (FD_ISSET(0, &_readfds))
        {
            char c = getchar();
            if (c == '\n')
            {
                std::cout << "Shutting down server" << std::endl;
                return (false);
            }
        }
    else if (selectRes > 0)
    {
        auto it = this->_sockets.begin();
        while (it != this->_sockets.end())
        {  
            const std::string socket_address = (*it)->getAddr();
            const unsigned short socket_port = (*it)->getPort();
            const int sckt = (*it)->getSocket();
            _hasError = false;
            if (FD_ISSET(sckt, &_errorfds))
            {
                std::cout << "Erreur" << std::endl;
                _hasError = true;
            }
            else 
            {
                if (FD_ISSET(sckt, &_readfds))
                    (*it)->handle(*this, Socket::READ);
                else if (FD_ISSET(sckt, &_writefds))
                    (*it)->handle(*this, Socket::WRITE);
            }
            if (_hasError)
            {
                std::cout << "Deconnexion de [" << socket_address << ":" << socket_port << "]" << std::endl;
                SocketClient * socketClient = dynamic_cast<SocketClient*>(it->get());
                if (socketClient){
                    // delete proprement que ce soit server ou user:
                    // tous les noms et les pointeurs dans toutes les maps (check_name, map des token...)
                    IRCServer::_observer->unsubscribe(socketClient);
                    IRCServer::_client_manager->deleteClient(socketClient, ClientManager::ClientChoice::ALL);

                }
                this->_sockets.erase(it);
            }
            ++it;
        }
    }
    return (true);
}

void SocketManager::dispatch(SocketServeur &serveur)
{
    this->addSocket(serveur.acceptNewClient());
}
void SocketManager::dispatch(SocketClient &client)
{
    _hasError = client.recvMessage();
}
void SocketManager::dispatch(Socket &socket)
{
    socket.readStdin();
};

void  SocketManager::writeToSocket(SocketClient &client) const{
    client.sendMessage();
}
