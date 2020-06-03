#include <iostream>
#include <sstream>
#include "IRCServer.class.hpp"
#include "MessageMediator.class.hpp"
#include "ReplyManager.class.hpp"
#include "ServerClient.class.hpp"
#include "Client.class.hpp"
#include "SocketManager.class.hpp"
#include "ChannelManager.class.hpp"
#include "IRCServer.class.hpp"
#include "User.class.hpp"
#include "Service.class.hpp"
#include "Observer.class.hpp"
#include "Utility.hpp"
MessageMediator::MessageMediator(void)
{
    this->_commands.insert({IRCMessage::IRCMessageType::NICK, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::IRCMessageType::SERVICE, &MessageMediator::createClient});
    this->_commands.insert({IRCMessage::IRCMessageType::USER, &MessageMediator::userCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::QUIT, &MessageMediator::quitCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::JOIN, &MessageMediator::joinCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::PASS, &MessageMediator::passCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::OPER, &MessageMediator::operCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::PART, &MessageMediator::partCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::PRIVMSG, &MessageMediator::privmsgCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::LUSERS, &MessageMediator::lusersCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::SERVER, &MessageMediator::createClient});
    return;
}

MessageMediator::~MessageMediator(void)
{
    return;
}

bool MessageMediator::handleMessage(IRCMessage const &message, SocketClient *socket)
{
    std::cout << RED << "Message mediator : " << RESET << std::endl;
    std::cout << message << std::endl;
    
    Client *origin = IRCServer::_client_manager->getClient(socket);
    IRCServer::_observer->setOriginOfMsg(origin);
    
    (this->*_commands[message.type])(message, socket);
    std::cout << "size of Client manager fter command : " << IRCServer::_client_manager->getSize(ClientManager::ClientChoice::ALL) << std::endl;
    return (true);
}

void MessageMediator::nickServerCommand(IRCMessage const &message, SocketClient *socket) const
{
    ServerClient *server_talk = static_cast<ServerClient *>(IRCServer::_client_manager->getClient(socket));
    bool valid = true;
    valid = (server_talk ? IRCServer::_client_manager->newUserFromServer(message, *server_talk) : false);
};

void MessageMediator::createClient(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = NULL;
    User *user = NULL;
    Service *service = NULL;
    ServerClient * server = NULL;
    if (message.type == IRCMessage::IRCMessageType::NICK)
    {
        if (message.params.token != 0)
            this->nickServerCommand(message, socket);
        else if (!message.getPrefix().empty()){
            ServerClient * server = IRCServer::getServerFromUser(message.params.prevNickname);
            if (!IRCServer::_client_manager->setNick(message.params.nickname, *(server->getUser(message.params.prevNickname))))
                return;
        }
        else
        {
            user = static_cast<User *>(IRCServer::_client_manager->getClient(socket));
            if (!user)
            {
                client = IRCServer::_client_manager->createAddClient(ClientManager::USER, socket, message.params.nickname);
                IRCServer::addUser(*static_cast<User*>(client), 1);
                static_cast<User*>(client)->setHostname(IRCServer::name);
            }
            else{
                if (!(IRCServer::_client_manager->setNick(message.params.nickname, *user)))
                    return;
            }
        }
    }
    else if (message.type == IRCMessage::IRCMessageType::SERVICE)
    {
        service = static_cast<Service *>(IRCServer::_client_manager->getClient(socket));
        if (!service)
            client = IRCServer::_client_manager->createAddClient(ClientManager::USER, socket, message.params.nickname);
        else
            IRCServer::_client_manager->setService(message.params.nickname, *service);
    }
    else if (message.type == IRCMessage::IRCMessageType::SERVER)
    {
        server = static_cast<ServerClient *>(IRCServer::_client_manager->getClient(socket));
        ServerClient *serverHost = static_cast<ServerClient *>(IRCServer::_client_manager->getClientByName(message.params.host));
        if (!message.params.host.empty() && message.params.host != message.params.newServer && !serverHost)
        {
            std::cout << "ERROR : MessageMediator line 100" << std::endl;
            exit(EXIT_SUCCESS);
        }
        if (message.params.uplink.empty() && message.params.hopcount > 1)
        {
            std::cout << "ERROR : MessageMediator line 105" << std::endl;
            exit(EXIT_SUCCESS);
        }
        if ((!server && !socket->getPassword().empty()))
        {
            client = IRCServer::_client_manager->createAddClient(ClientManager::SERVER, socket, message.params.newServer);
            if (client)
            {
                server = static_cast<ServerClient *>(client);
                IRCServer::_client_manager->setNewServer(message, *server, *server);
                IRCServer::replyToNewConnection(socket);
            }
        }
        else if (server && server->getName() != message.params.newServer)
        {
            ServerClient *new_server = static_cast<ServerClient*>(IRCServer::_client_manager->createClient(ClientManager::SERVER, socket, message.params.newServer));
            new_server ? IRCServer::_client_manager->setNewServer(message, *server, *new_server) : false;
        }
    }
}

void MessageMediator::userCommand(IRCMessage const &message, SocketClient *socket) const
{
    User *user = static_cast<User *>(IRCServer::_client_manager->getClient(socket));
    std::vector<std::string> parameters;
    if (user)
    {
        IRCServer::_client_manager->setUser(message, *user);
        IRCServer::addUser(*user, 1);
    }
}

void MessageMediator::quitCommand(IRCMessage const &message, SocketClient *socket) const
{
    static_cast<void>(message);
    IRCServer::_client_manager->deleteClient(socket, ClientManager::USER);
    IRCServer::_socket_manager->deleteSocket(socket);
}

void MessageMediator::joinCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = IRCServer::_client_manager->getClient(socket);
    if (client && client->status == Client::Status::CONNECTED)
        IRCServer::_channel_manager->handleJoinChannel(message, dynamic_cast<User *>(client));
}

void MessageMediator::passCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = IRCServer::_client_manager->getClient(socket);
    if (!client)
        socket->setPassword(message.params.password);
    else
        IRCServer::_reply_manager->reply(Parameters(), ReplyManager::ERR_ALREADYREGISTRED, socket);
}

void MessageMediator::operCommand(IRCMessage const &message, SocketClient *socket) const
{
    User *user = static_cast<User *>(IRCServer::_client_manager->getClient(socket));
    if (user && user->status == Client::Status::CONNECTED)
    {
        if (message.params.password == user->getPassword() && message.params.user == user->getUser())
        {
            user->addMode(User::o);
            user->setOper(*new Oper);
            Parameters param = {};
            IRCServer::_reply_manager->reply(param, ReplyManager::RPL_YOUREOPER, user->getSocketClient());
        }
    }
}

void MessageMediator::partCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = IRCServer::_client_manager->getClient(socket);
    if (client && client->status == Client::Status::CONNECTED)
        IRCServer::_channel_manager->handlePartChannel(message, dynamic_cast<User *>(client));
}

void MessageMediator::privmsgCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = IRCServer::_client_manager->getClient(socket);
    if (client && client->status == Client::Status::CONNECTED)
    {
        Channel *channel = IRCServer::_channel_manager->getChannel(message.params.target);
        if (channel)
            IRCServer::_channel_manager->sendMessageChannel(*static_cast<User *>(client), *channel, message.params.text);
        else
            IRCServer::_client_manager->sendMsg(*static_cast<User *>(client), message.params.text, message.params.target);
    }
}

bool MessageMediator::sendReply(std::string const &msg, SocketClient *socket) const
{
    std::cout << GREEN << "message sent : " << RESET << std::endl;
    std::cout << msg << std::endl;
    socket->addToQueue(msg);
    return true;
}

void MessageMediator::lusersCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = IRCServer::_client_manager->getClient(socket);
    (void)message;
    if (client && client->status == Client::Status::CONNECTED)
    {
        int clients = IRCServer::_client_manager->getSize(ClientManager::ClientChoice::ALL);
        int servers = IRCServer::_client_manager->getSize(ClientManager::ClientChoice::SERVER);
        int users = IRCServer::_client_manager->getSize(ClientManager::ClientChoice::USER);
        int channels = IRCServer::_channel_manager->getSize();
        std::ostringstream msg;
        msg << "Stats de IRC :" << std::endl;
        msg << "nombre de clients : " << clients << std::endl;
        msg << "nombre de serveurs (sans compter le serveur local)) : " << servers << std::endl;
        msg << "nombre de users : " << users << std::endl;
        msg << "nombre de channel : " << channels << std::endl;
        std::unordered_map<SocketClient *, Client *> clients_map = IRCServer::_client_manager->getClients();
        for (auto it = clients_map.begin(); it != clients_map.end(); it++)
        {
            if (dynamic_cast<User *>(it->second))
            {
                msg << "user : " << it->second->getName() << std::endl;
            }
            else if (dynamic_cast<ServerClient *>(it->second))
            {
                msg << "*******SERVER******" << std::endl;
                msg << *static_cast<ServerClient *>(it->second) << std::endl;
                msg << "*******************" << std::endl;
            }
        }
        this->sendReply(msg.str(), socket);
    }
}
