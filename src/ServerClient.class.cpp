#include "IRCServer.class.hpp"
#include "ServerClient.class.hpp"
#include "Utility.hpp"
#include "IRCMessage.class.hpp"
#include "MessageMediator.class.hpp"
#include <iostream>
ServerClient::ServerClient(SocketClient *socket) : Client(socket) {}

ServerClient::~ServerClient(void) {
    // delete les servers associés SI PAS D'AUTRES ROUTES DISPO! faire tourner algo maj table routage avant
    for (auto it = this->_users.begin(); it != this->_users.end(); ++it)
        IRCServer::_client_manager->deleteClient(it->second->getSocketClient(),  ClientManager::ClientChoice::ALL);
    this->_users.clear();
    IRCServer::deleteServer(this->_token);
}

ServerClient &ServerClient::setName(std::string const &name)
{
    this->_name = name;
    return *this;
}

void ServerClient::setServerInfo(Parameters const &params, Token const & token)
{
    this->_uplink = params.uplink;
    this->_token = token;
    this->_hopcount = params.hopcount + 1;
    this->_info = params.serverInfo;
}

std::string ServerClient::getUplink(void) const
{
    return this->_uplink;
}
unsigned int ServerClient::getHopcount(void) const
{
    return this->_hopcount;
}
unsigned int ServerClient::getToken(void) const
{
    return this->_token;
}

std::string ServerClient::getInfo(void) const
{
    return this->_info;
}

ServerClient *ServerClient::getServer(Token token) const
{
    auto it = _servers.find(token);
    if (it != _servers.end())
        return (it->second.server_client);
    return (NULL);
}

User *  ServerClient::getUser(std::string const & nickname) const{
    return this->_users.at(nickname);
}


void ServerClient::addServer(Token token, ServerClient &server, unsigned int hopcount)
{
    ServerClientLight light_server;
    light_server.hopcount = hopcount;
    light_server.server_client = &server;
    std::pair<Token, ServerClientLight> value(token, light_server);
    this->_servers.insert(value);
}

void ServerClient::addUser(User *user)
{
    std::pair<std::string, User*> value(user->getName(), user);
    this->_users.insert(value);
}

void ServerClient::setToken(Token token)
{
    this->_token = token;
};


std::unordered_map<Token, ServerClientLight> ServerClient::getServers() const
{
    return (this->_servers);
}

std::ostream &operator<<(std::ostream &o, ServerClient const &rhs)
{
    o << "server name : " << rhs.getName() << std::endl;
    o << "password : " << rhs.getPassword() << std::endl;
    o << "uplink : " << rhs.getUplink() << std::endl;
    o << "hopcount : " << rhs.getHopcount() << std::endl;
    o << "token : " << rhs.getToken() << std::endl;
    o << "info : " << rhs.getInfo() << std::endl;
    auto servers = rhs.getServers();
    if (!servers.empty())
    {
        o << "other servers : " << std::endl;
        for (auto i = servers.begin(); i != servers.end(); i++)
        {
            o << i->first << " : " << i->second.server_client->getName() << " : hopcount " << i->second.hopcount << std::endl;
        }
    }
    return o;
}
