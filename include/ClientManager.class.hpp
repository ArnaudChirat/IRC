#ifndef CLIENTMANAGER_CLASS_HPP
#define CLIENTMANAGER_CLASS_HPP
#include "Client.class.hpp"
#include "Socket.class.hpp"
#include "Service.class.hpp"
#include "User.class.hpp"
#include <set>
#include <unordered_map>
#include <map>

class ServerClient;
class IRCMessage;

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
    bool sendMsg(User &client, std::string const &msg, std::string const &target);
    bool setUser(std::string const &username, unsigned int mode, std::string real_name, User &client);
    bool setNick(std::string const &nick, User &client);
    bool setService(std::string const &nick, Service &client);
    bool setServerName(std::string const &name, ServerClient &server);
    bool setNewServer(IRCMessage const &, ServerClient &, ServerClient &);
    bool newUserFromServer(IRCMessage const &, ServerClient const &);
    void caseChangeNick(Client & client);

    void deleteClient(SocketClient *client, ClientChoice choice);
    Client *getClient(SocketClient *socket_client);
    std::vector<User*> getUsers();
    Client *getClientByName(std::string const &nick);
    int getSize(ClientChoice choice) const;
    std::unordered_map<SocketClient*, Client*> getClients() const;
    // void    dispatch();
private:
    void addClient(SocketClient *socket, Client *client, ClientChoice choice);
    bool checkName(ClientChoice choice, std::string const &name);
    //map avec doublon
    typedef std::pair<ClientChoice, std::string> Key;
    std::set<Key> _names_used;
    std::unordered_map<SocketClient*, Client*> _clients;
    std::multimap<std::string, Client*> _nick_clients;
    // std::list<std::unique_ptr<Client>> _clients;
};

#endif