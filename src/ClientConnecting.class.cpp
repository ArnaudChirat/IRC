#include "ClientConnecting.class.hpp"
#include "ClientConnected.class.hpp"
#include <iostream>
ClientConnecting::ClientConnecting(Client *context) : _context(context)
{
    std::cout << "Doit me connecte (User et Nick commande)" << std::endl;
    this->_ConnectingCommands.push(IRCMessageType::USER);
    this->_ConnectingCommands.push(IRCMessageType::NICK);
    //TODO mettre un timer avec thread
    return;
}

ClientConnecting::~ClientConnecting(void)
{
    return;
}

void ClientConnecting::handle(IRCMessage const &message)
{
    if (!this->_context->getNick().empty() && !this->_context->getUser().empty())
    {
        //TODO envoyer message au socket quand connecte
        this->_context->setState(new ClientConnected(this->_context));
    }
    else
    {
        std::cout << "je suis toujours entrain d'etre connecté" << std::endl;
        if (this->_ConnectingCommands.top() == message.type)
        {
            this->_ConnectingCommands.pop();
            if (message.type == IRCMessageType::NICK)
                this->_context->setNick(message.getParameters()[0]);
            if (message.type == IRCMessageType::USER)
                this->_context->setUser(message.getParameters()[0]);
        } else {
            std::cout << _ConnectingCommands.top() << " needed" << std::endl;
        }
    }
}