#include <iostream>
#include "IRCServer.class.hpp"
#include "MessageMediator.class.hpp"
#include "ReplyManager.class.hpp"
#include "SocketManager.class.hpp"
#include "ChannelManager.class.hpp"
#include "IRCServer.class.hpp"
#include "User.class.hpp"
MessageMediator::MessageMediator(void)
{
    this->_commands.insert({IRCMessage::NICK, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::SERVICE, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::USER, &MessageMediator::userCommand});
    this->_commands.insert({IRCMessage::QUIT, &MessageMediator::quitCommand});
    this->_commands.insert({IRCMessage::JOIN, &MessageMediator::joinCommand});
    this->_commands.insert({IRCMessage::PART, &MessageMediator::partCommand});
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
    std::cout << "size of Client manager fter command : " << IRCServer::_client_manager->getSize() << std::endl;
    // if ((message.type ::IRCMessage_TYPE_MASK) =::CONNECTION_REGISTRATION)
    //     this->createClient(message, socket);
    return (true);
}

void MessageMediator::createClient(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = NULL;
    User   *user = NULL;
    if (message.type == IRCMessage::NICK)
    {
        user = static_cast<User*>(IRCServer::_client_manager->getClient(socket));
        if (!user)
            client = IRCServer::_client_manager->createAddClient(ClientManager::USER, socket, message.params.nickname);
        else
        {
            IRCServer::_client_manager->setNick(message.params.nickname, socket);
        }
    }
    if (client)
        std::cout << "User created : " << client->getName() << std::endl;
    else if(user)
        std::cout << "User already exist nickname is : " << user->getName() << std::endl;
    else
        std::cout << "Nick name already use : " << message.params.nickname << std::endl;
}

void MessageMediator::userCommand(IRCMessage const &message, SocketClient *socket) const
{
    if (!IRCServer::_client_manager->setUser(message.params.user, socket))
        std::cout << "Nick not set or socket doesnt exist" << std::endl;
    else
        std::cout << "Client Connected" << std::endl;
}

void MessageMediator::quitCommand(IRCMessage const &message, SocketClient *socket) const
{
    std::cout << "quit command" << std::endl;
    IRCServer::_client_manager->deleteClient(socket, ClientManager::USER);
    IRCServer::_socket_manager->deleteSocket(socket);
    std::cout << "someone has quit" << message.params.quit_message  << std::endl;
}

void MessageMediator::joinCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client * client = IRCServer::_client_manager->getClient(socket);
    std::cout << "join command" << std::endl;
    // if (IRCServer::_channel_manager->verify(message, client))
    if (client){
        IRCServer::_channel_manager->handleJoinChannel(message, dynamic_cast<User*>(client));
    // dispay channels à virer asap
        IRCServer::_channel_manager->displayChannels();
    }
    
}

void    MessageMediator::partCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client * client = IRCServer::_client_manager->getClient(socket);
    if (client)
        IRCServer::_channel_manager->handlePartChannel(message, dynamic_cast<User*>(client));
}

bool    MessageMediator::sendReply(std::string const & msg, SocketClient * socket) const{
    socket->appendToBuffer(msg);
    return true;
}