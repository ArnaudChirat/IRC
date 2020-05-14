#include "ClientManager.class.hpp"
#include "ReplyManager.class.hpp"
#include "IRCServer.class.hpp"
#include "User.class.hpp"
#include "Utility.hpp"
#include <iostream>
ClientManager::ClientManager(void)
{
    return;
}

ClientManager::~ClientManager(void)
{
    return;
}

Client *ClientManager::createAddClient(ClientChoice choice, SocketClient *socket, std::string const &name)
{
    Client *client = this->createClient(choice, socket, name);
    if (client == NULL)
        return (NULL);
    this->addClient(socket, client, choice);
    return (client);
}

void ClientManager::addClient(SocketClient *socket, Client *client, ClientChoice choice)
{
    // this->_clients.emplace_back(client);
    this->_names_used.insert(Key(choice, client->getName()));
    this->_clients.insert(std::pair<SocketClient*, Client*>(socket, client));
}

Client *ClientManager::createClient(ClientChoice choice, SocketClient *socket, std::string const &name)
{
    Client *client = NULL;
    if (choice == USER)
        client = new User(socket);
    client->setName(name);
    if (checkName(choice, name)){
        //exemple de structure parameter crée avec un client
        Parameters parameters(*client);
        IRCServer::_reply_manager->reply(parameters, ReplyManager::ERR_NICKNAMEINUSE, socket);
        delete client; 
        return (NULL);
    }
    // if (choice == SERVER)
    //     client = new Server(socket_client);
    // if (choice == SERVICE)
    //     client = new Service(socket_client);
    return (client);
}

//Check if name already in use, return true if name already exist 
bool ClientManager::checkName(ClientChoice choice, std::string const &name)
{
    auto it = this->_names_used.find(Key(choice, name));
    if (it == this->_names_used.end())
        return (false);
    return (true);
}

Client *ClientManager::getClient(SocketClient *socket)
{
    auto it = this->_clients.find(socket);
    if (it != this->_clients.end())
        return it->second;    
    return NULL;
}

bool    ClientManager::setNick(std::string const &nick, SocketClient *socket)
{
    Client *client = this->getClient(socket);
    if (!client)
        return (false);
    if (checkName(USER, nick)){
        Parameters param = {};
        param.nickname = nick;
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NICKNAMEINUSE, socket);
        return (false);
    }
    this->_names_used.erase(Key(USER,client->getName()));
    client->setName(nick);
    this->_names_used.insert(Key(USER, nick));
    return (true);
};

bool    ClientManager::setUser(std::string const &username, SocketClient *socket_client)
{
    Client *client = this->getClient(socket_client);
    if (!client || client->getName().empty())
        return (false);
    User *user = static_cast<User*>(client);
    user->setUser(username);
    if (user->getUser().empty())
        return (false);
    client->status = Client::Status::CONNECTED;
    Parameters param(*user);
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_WELCOME, socket_client);
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_YOURHOST, socket_client);
    return (true);
};

void ClientManager::deleteClient(SocketClient *socket, ClientChoice choice)
{
    Client *client = this->getClient(socket);
    if (!client)
        return;
    Key key(choice, client->getName());
    this->_names_used.erase(key);
    this->_clients.erase(socket);
    delete client;
}

int ClientManager::getSize() const
{
    return (this->_names_used.size());
}