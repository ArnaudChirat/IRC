#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP
#include "Client.class.hpp"
#include "Utility.hpp"
#include <ostream>
#include <map>

struct Parameters;
struct ServerClientLight;
class ServerClient : public Client
{
public:
    ServerClient(SocketClient * socket);
    ServerClient(ServerClient const &instance) = delete;
    ServerClient &operator=(ServerClient const &rhs) = delete;
    ~ServerClient(void);
    ServerClient &setName(std::string const &name);
    void    setServerInfo(Parameters const & params);
    std::string  getUplink(void) const;
    unsigned int    getHopcount(void) const;
    unsigned int getToken(void) const;
    std::string getInfo(void) const;
    void addServer(Token token, ServerClient &server, unsigned int hopcount);
    void addServer(ServerClient &server);
    // void getServer(Token token);
    std::map<Token, ServerClientLight> getServers() const;
private:
    ServerClient(void);
    std::string     _uplink;
    unsigned int    _hopcount;
    unsigned int    _token;
    std::string     _info;
    std::map<Token, ServerClientLight> _servers;
};

struct ServerClientLight
{
    unsigned int hopcount;
    ServerClient *server_client;
};


std::ostream &   operator<<(std::ostream & o, ServerClient const & rhs);

#endif