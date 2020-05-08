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
    this->addClient(client);
    return (client);
}

void ClientManager::addClient(Client *client)
{
    this->_clients.emplace_back(client);
}

Client *ClientManager::createClient(ClientChoice choice, SocketClient *socket_client, std::string const &name)
{
    Client *client = NULL;
    if (!checkName(choice, name))
        return (NULL);
    if (choice == ClientChoice::USER)
        client = new User(socket_client);
    // if (choice == ClientChoice::SERVER)
    //     client = new Server(socket_client);
    // if (choice == ClientChoice::SERVICE)
    //     client = new Service(socket_client);
    client->setName(name);
    return (client);
}

bool ClientManager::checkName(ClientChoice type, std::string const &name)
{
    for (auto it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if (type == ClientChoice::USER && dynamic_cast<User*>(it->get()) && (*it)->getName() == name)
            return (false);
        // if (type == ClientChoice::SERVER && dynamic_cast<Server*>(it->get()) && (*it)->getName() == name)
        //     return (false);
        // if (type == ClientChoice::SERVICE && dynamic_cast<Service*>(it->get()) && (*it)->getName() == name)
        //     return (false);
    }
    return (true);
}

Client *ClientManager::getClient(SocketClient *socket_client)
{
    for (auto it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        Client *client = it->get();
        if (client->_socket_client == socket_client)
            return client;
    }
    return (NULL);
}

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
    return (true);
};
