#include "ClientManager.class.hpp"
#include "IRCServer.class.hpp"
#include "User.class.hpp"
#include "Service.class.hpp"
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
    (void)choice;
    // this->_names_used.insert(Key(choice, client->getName()));
    this->_clients.insert(std::pair<SocketClient *, Client *>(socket, client));
}

Client *ClientManager::createClient(ClientChoice choice, SocketClient *socket, std::string const &name)
{
    Client *client = NULL;
    bool not_error = true;
    if (choice == ClientChoice::USER)
    {
        client = new User(socket);
        not_error = setNick(name, *client);
    }
    if (choice == ClientChoice::SERVICE)
    {
        client = new Service(socket);
        not_error = setService(name, *client);
    }
    if (!not_error)
    {
        delete client;
        return (NULL);
    }
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
    return (this->_clients.find(socket)->second);
}

bool ClientManager::setNick(std::string const &nick, Client &client)
{
    if (checkName(USER, nick))
    {
        std::string oldNick = client.getName();
        client.setName(nick);
        IRCServer::_reply_manager.errorReply(&client, NULL, ReplyManager::ERR_NICKNAMEINUSE);
        client.setName(oldNick);
        return (false);
    }
    this->_names_used.erase(Key(USER, client.getName()));
    client.setName(nick);
    this->_names_used.insert(Key(USER, nick));
    return (true);
};

bool ClientManager::setService(std::string const &nick, Client &client)
{
    std::string real_name(nick + IRCServer::name);
    if (checkName(SERVICE, real_name))
    {
        std::string old_service = client.getName();
        client.setName(real_name);
        IRCServer::_reply_manager.errorReply(&client, NULL, ReplyManager::ERR_ALREADYREGISTRED);
        client.setName(old_service);
        return (false);
    }
    this->_names_used.erase(Key(SERVICE, client.getName()));
    client.setName(real_name);
    this->_names_used.insert(Key(SERVICE, real_name));
    IRCServer::_reply_manager.connectionReply(&client, ReplyManager::RPL_WELCOME);
    IRCServer::_reply_manager.connectionReply(&client, ReplyManager::RPL_YOURHOST);
    return (true);
};

bool ClientManager::setUser(std::string const &username, SocketClient *socket_client)
{
    Client *client = this->getClient(socket_client);
    if (!client || client->getName().empty())
        return (false);
    User *user = static_cast<User *>(client);
    user->setUser(username);
    if (user->getUser().empty())
        return (false);
    client->status = Client::Status::CONNECTED;
    IRCServer::_reply_manager.connectionReply(client, ReplyManager::RPL_WELCOME);
    IRCServer::_reply_manager.connectionReply(client, ReplyManager::RPL_YOURHOST);
    return (true);
};

void ClientManager::deleteClient(SocketClient *socket, ClientChoice choice)
{
    Client *client = this->getClient(socket);
    if (!client)
        return;
    if (choice == ClientChoice::ALL)
    {
        if (dynamic_cast<User *>(client))
            choice = ClientManager::USER;
        if (dynamic_cast<Service *>(client))
            choice = ClientManager::SERVICE;
    }
    Key key(choice, client->getName());
    this->_names_used.erase(key);
    this->_clients.erase(socket);
    delete client;
}

int ClientManager::getSize() const
{
    return (this->_names_used.size());
}