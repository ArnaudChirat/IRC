#include "Client.class.hpp"
#include <iostream>
Client::Client(void)
{
    return;
}

Client::Client(int const socket, sockaddr_in const &addr) : Socket(socket, addr)
{
    return;
}

Client::~Client(void)
{
}

bool Client::recvMessage() {
    char buffer[200] = {0};
    int sckt = this->getSocket();
    int ret = recv(sckt, buffer, 199, 0);
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
        std::cout << "[" << this->getAddr().c_str() << ":" << this->getPort() << "] " << buffer;
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
