#include "IRCServer.class.hpp"
#include "MessageMediator.class.hpp"
#include "ReplyManager.class.hpp"
#include <iostream>
#include "IRCServer.class.hpp"
#include "User.class.hpp"
#include "Service.class.hpp"
MessageMediator::MessageMediator(void)
{
    this->_commands.insert({IRCMessage::NICK, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::SERVICE, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::USER, &MessageMediator::userCommand});
    this->_commands.insert({IRCMessage::QUIT, &MessageMediator::quitCommand});
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
    // if ((message.type ::IRCMessage_TYPE_MASK) =::CONNECTION_REGISTRATION)
    //     this->createClient(message, socket);
    return (true);
}

void MessageMediator::createClient(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = NULL;
    User   *user = NULL;
    Service *service = NULL;
    if (message.type == IRCMessage::NICK)
    {
        user = static_cast<User*>(IRCServer::_client_manager.getClient(socket));
        if (!user)
            client = IRCServer::_client_manager.createAddClient(ClientManager::USER, socket, message.getParameters()[0]);
        else
        {
            IRCServer::_client_manager.setNick(message.getParameters()[0], *user);
        }
    }
    if (message.type == IRCMessage::SERVICE)
    {
        service = static_cast<Service*>(IRCServer::_client_manager.getClient(socket));
        if (!service)
            client = IRCServer::_client_manager.createAddClient(ClientManager::SERVICE, socket, message.getParameters()[0]);
        else
        {
            IRCServer::_client_manager.setService(message.getParameters()[0], *service);
        }
    }
    if (client)
        std::cout << "Client created : " << client->getName() << std::endl;
    else if(user)
        std::cout << "User already exist nickname is : " << client->getName() << std::endl;
    else
        std::cout << "Nick name already use : " << message.getParameters()[0] << std::endl;
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
    IRCServer::_client_manager.deleteClient(socket, ClientManager::ClientChoice::ALL);
    IRCServer::_socket_manager.deleteSocket(socket);
    std::cout << "someone has quit" << (message.getParameters().empty() ?  "" : message.getParameters()[0])  << std::endl;
}

bool    MessageMediator::sendReply(std::string const & msg, Client * client) const{
    client->getSocketClient()->appendToBuffer(msg);
    return true;
}