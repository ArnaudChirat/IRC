#include "ClientManager.class.hpp"
#include "ReplyManager.class.hpp"
#include "IRCServer.class.hpp"
#include "User.class.hpp"
#include "ServerClient.class.hpp"
#include "Service.class.hpp"
#include "Observer.class.hpp"
#include "Utility.hpp"
#include <algorithm>
#include <iostream>
#include <algorithm>
ClientManager::ClientManager(void)
{
    return;
}

ClientManager::~ClientManager(void)
{
    return;
}

bool ClientManager::newUserFromServer(IRCMessage const & message, ServerClient const & server_talk) {
    User *user = static_cast<User*>(IRCServer::_client_manager->createClient(ClientManager::USER, NULL, message.params.nickname));
    ServerClient * server = server_talk.getServer(message.params.token);
    server->addUser(user);
    user->setHostname(message.params.host);
    IRCServer::addUser(*user, server->getToken());
    if(!setUser(message, *user))
        return false;
    return true;
}

bool ClientManager::setNewServer(IRCMessage const & msg, ServerClient & server, ServerClient & newServer){
    Token ourToken = IRCServer::addServer(newServer);
    newServer.setServerInfo(msg.params, ourToken);
    server.addServer(msg.params.token, newServer, msg.params.hopcount);
    if (msg.params.hopcount == 1)
        IRCServer::_observer->subscribe(newServer.getSocketClient());
    newServer.status = Client::Status::CONNECTED;
    IRCServer::_observer->notify(&newServer, "SERVER");
    return true;
}

Client *ClientManager::createAddClient(ClientChoice choice, SocketClient *socket, std::string const &name)
{
    Client *client = this->createClient(choice, socket, name);
    if (client == NULL)
        return (NULL);
    this->addClient(socket, client, choice);
    return (client);
}

std::unordered_map<SocketClient *, Client *> ClientManager::getClients() const
{
    return (this->_clients);
}

std::vector<User*> ClientManager::getUsers()
{
    std::vector<User*> users;
    for (auto i = _clients.begin(); i != _clients.end() ; i++)
    {
        User *user = dynamic_cast<User*>(i->second);
        if (user && user->status == User::Status::CONNECTED)
        {
            users.push_back(user);
        }
    }
    return (users);
};


void ClientManager::addClient(SocketClient *socket, Client *client, ClientChoice choice)
{
    // this->_clients.emplace_back(client);
    (void)choice;
    // this->_names_used.insert(Key(choice, client->getName()));
    if (_clients.find(socket) == _clients.end())
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
    else if (choice == ClientChoice::SERVER)
    {
        client = new ServerClient(socket);
        not_error = setServerName(name, *static_cast<ServerClient *>(client));
    }
    if (!not_error)
    {
        delete client;
        return (NULL);
    }
    if (socket)
        client->setPassword(socket->getPassword());
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

bool ClientManager::setServerName(std::string const &name, ServerClient &server)
{
    if (checkName(SERVER, name))
    {
        IRCServer::_reply_manager->reply(Parameters(), ReplyManager::ERR_ALREADYREGISTRED, server.getSocketClient());
        return (false);
    }
    server.setName(name);
    this->_names_used.insert(Key(SERVER, name));
    return (true);
};

void ClientManager::caseChangeNick(Client & client){
    ServerClient * server = IRCServer::getServerFromUser(client.getPrevName());
    User * user  = dynamic_cast<User*>(&client);
    IRCServer::removeUser(client.getPrevName());
    if (server){
        server->removeUser(client.getPrevName());
        server->addUser(user);
        IRCServer::addUser(*user, server->getToken());
    }
    else
        IRCServer::addUser(*user, 1);
    IRCServer::_observer->notify(&client, "NICK");
    client.setPrevName("");
}


bool ClientManager::setNick(std::string const &nick, User &client)
{
    client.setPrevName(client.getName());
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
    if (client.status == Client::Status::CONNECTED)
        this->caseChangeNick(client);
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

bool ClientManager::setUser(IRCMessage const & message, User &client)
{
    if (client.getName().empty())
        return (false);
    client.setUser(message.params.user);
    if (client.getUser().empty())
        return (false);
    unsigned int mode = (!message.params.modestr.empty() ? client.getModeIntFromStr(message.params.modestr) : message.params.modeint);
    if (IRCServer::getTokenFromUser(client.getName()) == 1)
        mode = (mode & (User::w | User::i));
    client.setRealName(message.params.real_name).setHostname(message.params.host).setMode(mode);
    client.status = Client::Status::CONNECTED;
    if (IRCServer::getTokenFromUser(client.getName()) == 1){
        Parameters param(client);
        IRCServer::_reply_manager->reply(param, ReplyManager::RPL_WELCOME, client.getSocketClient());
        IRCServer::_reply_manager->reply(param, ReplyManager::RPL_YOURHOST, client.getSocketClient());
    }
    IRCServer::_observer->notify(&client, "NICK");
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
        if (dynamic_cast<ServerClient *>(client))
            choice = ClientManager::SERVER;
    }
    std::string name = client->getName();
    Key key(choice, name);
    this->_names_used.erase(key);
    this->_clients.erase(socket);
    this->_nick_clients.erase(name);
    delete client;
}

int ClientManager::getSize(ClientChoice choice) const
{
    unsigned long total_size = this->_names_used.size();
    std::set<Key> list;
    if (choice == ClientChoice::ALL)
        return (total_size);
    else
    {
        std::copy_if(this->_names_used.begin(), this->_names_used.end(), std::inserter(list, list.end()), [choice](Key const &key) { return (key.first == choice); });
        return (list.size());
    }
}

bool ClientManager::sendMsg(User &client, std::string const &msg, std::string const &target)
{
    User *target_ptr = NULL;
    if (client.getName() != target && checkName(USER, target))
    {
        target_ptr = static_cast<User *>(this->getClientByName(target));
        if (target_ptr->status == Client::Status::CONNECTED)
            client.sendMsgTo(target_ptr, msg);
        else
        {
            Parameters param = {};
            param.nickname = target;
            IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOSUCHNICK, client.getSocketClient());
        }
    }
    return (false);
};