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
    Client  *createClient(ClientChoice choice, SocketClient *socket_client, std::string const &name);
    bool    setUser(std::string const &username, SocketClient *socket_client);
    // void    dispatch();
private:
    void    addClient(Client *client);
    bool    checkName(ClientChoice type, std::string const &name);
    Client *getClient(SocketClient *socket_client);
    std::list<std::unique_ptr<Client>> _clients;
};

#endif