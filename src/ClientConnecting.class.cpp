#include "ClientConnecting.class.hpp"
#include "ClientConnected.class.hpp"
#include <iostream>
ClientConnecting::ClientConnecting(Client *context) : _context(context)
{
    std::cout << "Doit me connecte (User et Nick commande)" << std::endl;
    return;
}

ClientConnecting::~ClientConnecting(void)
{
    return;
}

void ClientConnecting::handle()
{
    if (!this->_context->getNick().empty() && !this->_context->getUser().empty())
        this->_context->setState(new ClientConnected(this->_context));
    else
        std::cout << "je suis toujours entrain d'etre connecté" << std::endl;
}