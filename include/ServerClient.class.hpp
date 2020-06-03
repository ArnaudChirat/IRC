#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP
#include "Client.class.hpp"
#include "Utility.hpp"
#include <ostream>
#include <unordered_map>

struct Parameters;
struct ServerClientLight;
struct UserClientLight;
class ServerClient : public Client
{
public:
    ServerClient(SocketClient * socket);
    ServerClient(ServerClient const &instance) = delete;
    ServerClient &operator=(ServerClient const &rhs) = delete;
    ~ServerClient(void);
    ServerClient &setName(std::string const &name);
    void    setServerInfo(Parameters const & params, Token const &);
    std::string  getUplink(void) const;
    unsigned int    getHopcount(void) const;
    unsigned int getToken(void) const;
    void setToken(Token token);
    std::string getInfo(void) const;
    void addServer(Token token, ServerClient &server, unsigned int hopcount);
    void addServer(ServerClient &server);
    void addUser(User * user);
    void removeUser(std::string const &);
    ServerClient *getServer(Token token) const;
    User * getUser(std::string const &) const;
    std::vector<User *> getUsers() const;
    std::unordered_map<Token, ServerClientLight> getServers() const;
    
private:
    ServerClient(void);
    std::string     _uplink;
    unsigned int    _hopcount;
    unsigned int    _token;
    std::string     _info;
    std::unordered_map<Token, ServerClientLight> _servers;
    std::unordered_map<std::string, User*> _users;
};

struct ServerClientLight
{
    unsigned int hopcount;
    ServerClient *server_client;
};


std::ostream &   operator<<(std::ostream & o, ServerClient const & rhs);

#endif