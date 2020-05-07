#ifndef CLIENTMANAGER_CLASS_HPP
#define CLIENTMANAGER_CLASS_HPP
#include "Client.class.hpp"
#include "Socket.class.hpp"
#include <list>
class ClientManager
{
public:
    enum ClientChoice
    {
        USER,
        SERVER,
        SERVICE
    };
    ClientManager(void);
    ClientManager(ClientManager const &instance);
    ClientManager &operator=(ClientManager const &rhs) = delete;
    ~ClientManager(void);
    Client  *createAddClient(ClientChoice choice, SocketClient *socket_client, std::string const &name);
    void    addClient(Client *client);
    Client  *createClient(ClientChoice choice, SocketClient *socket_client, std::string const &name);
    bool    checkName(ClientChoice type, std::string const &name);
    // void    dispatch();
private:
    std::list<std::unique_ptr<Client>> _clients;
};

#endif