#include "MessageMediator.class.hpp"
#include <iostream>
#include "IRCServer.class.hpp"
#include "User.class.hpp"
MessageMediator::MessageMediator(void)
{
    this->_commands.insert({IRCMessageType::NICK, &MessageMediator::createClient});
    this->_commands.insert({IRCMessageType::SERVICE, &MessageMediator::createClient});
    this->_commands.insert({IRCMessageType::USER, &MessageMediator::userCommand});
    this->_commands.insert({IRCMessageType::QUIT, &MessageMediator::quitCommand});
    return;
}

MessageMediator::~MessageMediator(void)
{
    return;
}

bool MessageMediator::handleMessage(IRCMessage const &message, SocketClient *socket)
{
    std::cout << "Message mediator : " << std::endl;
    std::cout << message << std::endl;
    // std::cout << (this->_commands.at(message.type)) << std::endl;
    (this->*_commands[message.type])(message, socket);
    std::cout << "size of Client manager fter command : " << IRCServer::_client_manager.getSize() << std::endl;
    // if ((message.type & IRCMessageType::IRCMessage_TYPE_MASK) == IRCMessageType::CONNECTION_REGISTRATION)
    //     this->createClient(message, socket);
    return (true);
}

void MessageMediator::createClient(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = NULL;
    User   *user = NULL;
    if (message.type == IRCMessageType::NICK)
    {
        user = static_cast<User*>(IRCServer::_client_manager.getClient(socket));
        if (!user)
            client = IRCServer::_client_manager.createAddClient(ClientManager::USER, socket, message.getParameters()[0]);
        else
            user->setName(message.getParameters()[0]);
    }
    if (client)
        std::cout << "User created : " << client->getName() << std::endl;
    else
        std::cout << "User already exist nickname is : " << user->getName() << std::endl;
}

void MessageMediator::userCommand(IRCMessage const &message, SocketClient *socket) const
{
    if (!IRCServer::_client_manager.setUser(message.getParameters()[0], socket))
        std::cout << "Nick not set or socket doesnt exist" << std::endl;
    else
        std::cout << "Client Connected" << std::endl;
}

void MessageMediator::quitCommand(IRCMessage const &message, SocketClient *socket) const
{
    std::cout << "quit command" << std::endl;
    IRCServer::_client_manager.deleteClient(socket);
    IRCServer::_socket_manager.deleteSocket(socket);
    std::cout << "someone has quit" << (message.getParameters().empty() ?  "" : message.getParameters()[0])  << std::endl;
}