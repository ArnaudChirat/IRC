#ifndef CLIENTMANAGER_CLASS_HPP
#define CLIENTMANAGER_CLASS_HPP
#include "Client.class.hpp"
#include "Socket.class.hpp"
#include <set>
#include <map>
class ClientManager
{
public:
    enum ClientChoice
    {
        USER,
        SERVER,
        SERVICE,
        ALL
    };
    ClientManager(void);
    ClientManager(ClientManager const &instance);
    ClientManager &operator=(ClientManager const &rhs) = delete;
    ~ClientManager(void);
    Client *createAddClient(ClientChoice choice, SocketClient *socket_client, std::string const &name);
    Client *createClient(ClientChoice choice, SocketClient *socket_client, std::string const &name);
    bool setUser(std::string const &username, SocketClient *socket_client);
    bool setNick(std::string const &nick, SocketClient *socket_client);
    bool setService(std::string const &nick, SocketClient *socket_client);
    void deleteClient(SocketClient *client, ClientChoice choice);
    Client *getClient(SocketClient *socket_client);
    int getSize() const;
    // void    dispatch();
private:
    void addClient(SocketClient *socket, Client *client, ClientChoice choice);
    bool checkName(ClientChoice choice, std::string const &name);
    //map avec doublon
    typedef std::pair<ClientChoice, std::string> Key;
    std::set<Key> _names_used;
    std::multimap<SocketClient*, Client*> _clients;
    // std::list<std::unique_ptr<Client>> _clients;
};

#endif