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
#include "RoutingTable.class.hpp"
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
    this->_commands.insert({IRCMessage::IRCMessageType::SQUIT, &MessageMediator::squitCommand});
    this->_commands.insert({IRCMessage::IRCMessageType::NJOIN, &MessageMediator::njoinCommand});
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
    
    IRCServer::_observer->setOriginOfMsg(socket);
    (this->*_commands[message.type])(message, socket);
    IRCServer::_observer->setOriginOfMsg(NULL);
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
            throw std::logic_error("ERROR : MessageMediator line 100");
        if (message.params.uplink.empty() && message.params.hopcount > 1)
            throw std::logic_error("ERROR : MessageMediator line 105");
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
    User * user = NULL;
    if (message.params.nickname.empty()){
        user = dynamic_cast<User*>(IRCServer::_client_manager->getClient(socket));
        user->setMessage(message.params.quit_message);
        IRCServer::_observer->notify(user, "QUIT");
        IRCServer::_client_manager->deleteClient(socket, ClientManager::USER);
        IRCServer::_socket_manager->deleteSocket(socket);
    }
    else {
        ServerClient * server = IRCServer::getServerFromUser(message.params.nickname);
        user = server->getUser(message.params.nickname);
        user->setMessage(message.params.quit_message);
        IRCServer::_observer->notify(user, "QUIT");
        server->removeUser(message.params.nickname);
        IRCServer::_client_manager->deleteClient(user, ClientManager::USER);
    }
}

void MessageMediator::joinCommand(IRCMessage const &message, SocketClient *socket) const
{
    Client *client = IRCServer::_client_manager->getClient(socket);
    if (client && client->status == Client::Status::CONNECTED)
        IRCServer::_channel_manager->handleJoinChannel(message, dynamic_cast<User *>(client));
}

void MessageMediator::njoinCommand(IRCMessage const &message, SocketClient *socket) const
{
    ServerClient *server = dynamic_cast<ServerClient*>(IRCServer::_client_manager->getClient(socket));
    if (server){
        IRCServer::_channel_manager->handleNJoin(message);
    }
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

void MessageMediator::squitCommand(IRCMessage const &message, SocketClient *socket) const
{
    (void)socket;
    if (dynamic_cast<ServerClient*>(IRCServer::_client_manager->getClient(socket))){ // si le message nous vient d'un server
        ServerClient * server = IRCServer::getAnyServerByName(message.params.name);
        if (server) {
            IRCServer::_observer->notify(server, "SQUIT");
            std::cout << server->getName() << std::endl;
            IRCServer::_client_manager->deleteClient(server, ClientManager::ClientChoice::SERVER);
        }
    }
    else {
        throw std::logic_error("Squit message not coming from server : FORBIDDEN!");
    }
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
            IRCServer::_client_manager->sendMsg(client, message);
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
        std::vector<ServerClient*> servers = IRCServer::getServers();
        std::vector<User*> users = IRCServer::getUsers();
        IRCServer *local = IRCServer::getInstance();
        int channels = IRCServer::_channel_manager->getSize();
        std::ostringstream msg;
        msg << "Stats de IRC :" << std::endl;
        msg << "nombre de users : " << users.size() << std::endl;
        msg << "nombre de serveurs : " << servers.size() << std::endl;
        msg << "nombre de channel : " << channels << std::endl;
        msg << "Serveur local :" << std::endl;
        msg << "Nom : " << local->name << std::endl;
        msg << "password : " << local->getPassword() << std::endl;
        msg << "****** Autre serveurs *******" << std::endl;
        for (auto i = servers.begin(); i != servers.end(); i++)
        {
            msg << **i << std::endl;
            msg << "\t*\t" << std::endl;
        }
        msg << "****** Utilisateurs *******" << std::endl;
        for (auto i = users.begin(); i != users.end(); i++)
        {
            msg << **i << std::endl;
            msg << "\t*\t" << std::endl;
        }
        msg << "****** Table de routage *******" << std::endl;
        msg << *IRCServer::_routing_table << std::endl;
        this->sendReply(msg.str(), socket);
    }
}
