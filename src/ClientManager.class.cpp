#include "ClientManager.class.hpp"
#include "User.class.hpp"
#include <iostream>
ClientManager::ClientManager(void)
{
    return;
}

ClientManager::~ClientManager(void)
{
    return;
}

Client *ClientManager::createAddClient(ClientChoice choice, SocketClient *socket_client, std::string const &name)
{
    Client *client = this->createClient(choice, socket_client, name);
    if (client == NULL)
        return (NULL);
    client->setName(name);
    this->addClient(client);
    return (client);
}

void ClientManager::addClient(Client *client)
{
    this->_clients.emplace_back(client);
}

Client *ClientManager::createClient(ClientChoice choice, SocketClient *socket_client, std::string const &name)
{
    if (!checkName(choice, name))
        return (NULL);
    if (choice == ClientChoice::USER)
        return  new User(socket_client);
    return (NULL);
}

bool ClientManager::checkName(ClientChoice type, std::string const &name)
{
    for (auto it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if (type == ClientChoice::USER && dynamic_cast<User*>(it->get()) && (*it)->getName() == name)
            return (false);
    }
    return (true);
}

