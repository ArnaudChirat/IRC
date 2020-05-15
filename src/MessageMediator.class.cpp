#include "IRCServer.class.hpp"
#include "MessageMediator.class.hpp"
#include "ReplyManager.class.hpp"
#include <iostream>
#include <sstream>
#include "IRCServer.class.hpp"
#include "User.class.hpp"
#include "Service.class.hpp"
MessageMediator::MessageMediator(void)
{
    this->_commands.insert({IRCMessage::NICK, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::SERVICE, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::USER, &MessageMediator::userCommand});
    this->_commands.insert({IRCMessage::QUIT, &MessageMediator::quitCommand});
    this->_commands.insert({IRCMessage::JOIN, &MessageMediator::joinCommand});
    this->_commands.insert({IRCMessage::PASS, &MessageMediator::passCommand});
    this->_commands.insert({IRCMessage::OPER, &MessageMediator::operCommand});
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
    User *user = NULL;
    Service *service = NULL;
    if (message.type == IRCMessage::NICK)
    {
        user = static_cast<User *>(IRCServer::_client_manager.getClient(socket));
        if (!user)
            client = IRCServer::_client_manager.createAddClient(ClientManager::USER, socket, message.parameters_struct.nickname);
        else
            IRCServer::_client_manager.setNick(message.parameters_struct.nickname, *user);
    }
    if (message.type == IRCMessage::SERVICE)
    {
        service = static_cast<Service *>(IRCServer::_client_manager.getClient(socket));
        if (!service)
            client = IRCServer::_client_manager.createAddClient(ClientManager::USER, socket, message.parameters_struct.nickname);
        else
            IRCServer::_client_manager.setService(message.parameters_struct.nickname, *service);
    }
    if (client)
        std::cout << "User created : " << client->getName() << std::endl;
    else if(user)
        std::cout << "User already exist nickname is : " << user->getName() << std::endl;
    else
        std::cout << "Nick name already use : " << message.parameters_struct.nickname << std::endl;
}

void MessageMediator::userCommand(IRCMessage const &message, SocketClient *socket) const
{
    User *user = static_cast<User *>(IRCServer::_client_manager.getClient(socket));
    std::vector<std::string> parameters;
    if (user)
        IRCServer::_client_manager.setUser(message.parameters_struct.user, message.parameters_struct.mode, message.getTrail(), *user);
    else
        std::cout << "Nick not set or socket doesnt exist" << std::endl;
}

void MessageMediator::quitCommand(IRCMessage const &message, SocketClient *socket) const
{
    std::cout << "quit command" << std::endl;
    IRCServer::_client_manager.deleteClient(socket, ClientManager::ClientChoice::ALL);
    IRCServer::_socket_manager.deleteSocket(socket);
    std::cout << "someone has quit" << message.parameters_struct.quit_message  << std::endl;
}

void MessageMediator::joinCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client * user = IRCServer::_client_manager.getClient(socket);
    std::cout << "join command" << std::endl;
    // if (IRCServer::_channel_manager.verify(message, user))
    if (user){
        IRCServer::_channel_manager.handleJoinChannel(message, dynamic_cast<User*>(user));
    // dispay channels à virer asap
        IRCServer::_channel_manager.displayChannels();
    }
    
}

void MessageMediator::passCommand(IRCMessage const &message, SocketClient *socket) const
{
    User * user = static_cast<User*>(IRCServer::_client_manager.getClient(socket));
    std::cout << "pass command" << std::endl;
    // if (IRCServer::_channel_manager.verify(message, user))
    if (user){
        user->setPassword(message.parameters_struct.password);
    }
    
}

void MessageMediator::operCommand(IRCMessage const &message, SocketClient *socket) const
{
    User * user = static_cast<User*>(IRCServer::_client_manager.getClient(socket));
    std::cout << "oper command" << std::endl;
    // if (IRCServer::_channel_manager.verify(message, user))
    if (user){
        if (message.parameters_struct.password == user->getPassword() && message.parameters_struct.user == user->getUser())
        {
            user->addMode(User::o);
            user->setOper(*new Oper);
            IRCServer::_reply_manager.commandReply(user, NULL, ReplyManager::RPL_YOUREOPER);
        }
    }
    
}


bool    MessageMediator::sendReply(std::string const & msg, Client * client) const{
    client->getSocketClient()->appendToBuffer(msg);
    return true;
}