#include "ClientConnected.class.hpp"
#include <iostream>

ClientConnected::ClientConnected(Client *context) : _context(context)
{
    std::cout << "Je suis enregistré" << std::endl;
    return;
}

ClientConnected::~ClientConnected(void)
{
    return;
}

void ClientConnected::handle()
{
    std::cout << "Handle du conecté" << std::endl;
}