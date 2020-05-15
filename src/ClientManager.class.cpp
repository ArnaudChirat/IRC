#include "ClientManager.class.hpp"
#include "ReplyManager.class.hpp"
#include "IRCServer.class.hpp"
#include "User.class.hpp"
#include "Service.class.hpp"
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
    (void)choice;
    // this->_names_used.insert(Key(choice, client->getName()));
    this->_clients.insert(std::pair<SocketClient *, Client *>(socket, client));
    this->_nick_clients.insert(std::pair<std::string, Client *>(client->getName(), client));
}

Client *ClientManager::createClient(ClientChoice choice, SocketClient *socket, std::string const &name)
{
    Client *client = NULL;
    bool not_error = true;

    if (choice == USER)
    {
        client = new User(socket);
        not_error = setNick(name, *static_cast<User *>(client));
    }
    else if (choice == ClientChoice::SERVICE)
    {
        client = new Service(socket);
        not_error = setService(name, *static_cast<Service *>(client));
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
    auto it = this->_clients.find(socket);
    if (it != this->_clients.end())
        return it->second;
    return NULL;
}

Client *ClientManager::getClientByName(std::string const &name)
{
    auto it = this->_nick_clients.find(name);
    if (it != this->_nick_clients.end())
        return it->second;
    return NULL;
}

bool ClientManager::setNick(std::string const &nick, User &client)
{
    if (checkName(USER, nick))
    {
        Parameters param = {};
        param.nickname = nick;
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NICKNAMEINUSE, client.getSocketClient());
        return (false);
    }
    this->_names_used.erase(Key(USER, client.getName()));
    client.setName(nick);
    if (client.getName().empty())
    {
        Parameters param = {};
        param.nickname = nick;
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_ERRONEUSNICKNAME, client.getSocketClient());
    }
    this->_names_used.insert(Key(USER, nick));
    return (true);
};

bool ClientManager::setService(std::string const &nick, Service &client)
{
    std::string real_name(nick + IRCServer::name);
    if (checkName(SERVICE, real_name))
    {
        Parameters param = {};
        param.nickname = nick;
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NICKNAMEINUSE, client.getSocketClient());
        return (false);
    }
    this->_names_used.erase(Key(SERVICE, client.getName()));
    client.setName(real_name);
    if (client.getName().empty())
    {
        Parameters param = {};
        param.nickname = nick;
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_ERRONEUSNICKNAME, client.getSocketClient());
    }
    this->_names_used.insert(Key(SERVICE, real_name));
    Parameters param(client);
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_WELCOME, client.getSocketClient());
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_YOURHOST, client.getSocketClient());
    return (true);
};

bool ClientManager::setUser(std::string const &username, unsigned int mode, std::string real_name, User &client)
{
    if (client.getName().empty())
        return (false);
    client.setUser(username);
    if (client.getUser().empty())
        return (false);
    mode = (mode & (User::w | User::i));
    client.setRealName(real_name).setHostname(IRCServer::name).setMode(mode);
    client.status = Client::Status::CONNECTED;
    Parameters param(client);
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_WELCOME, client.getSocketClient());
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_YOURHOST, client.getSocketClient());
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
    this->_nick_clients.erase(client->getName());
    delete client;
}

int ClientManager::getSize() const
{
    return (this->_names_used.size());
}

bool ClientManager::sendMsg(User &client, std::string const &msg, std::string const &target)
{
    User *targer_ptr = NULL;
    if (client.getName() != target && checkName(USER, target))
    {
        targer_ptr = static_cast<User *>(this->getClientByName(target));
        if (targer_ptr->status == Client::Status::CONNECTED)
        {
            std::string sendingmsg;
            std::string prefix = ":" + client.getName() + "!" + IRCServer::name;
            sendingmsg = prefix + " PRIVMSG " + target + " :" + msg + "\n";
            IRCServer::_message_mediator->sendReply(sendingmsg, targer_ptr->getSocketClient());
        }
        else
        {
            Parameters param = {};
            param.nickname = target;
            IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOSUCHNICK, client.getSocketClient());
        }
    }
    return (false);
};