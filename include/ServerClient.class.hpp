#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include "Client.class.hpp"
#include <ostream>

struct Parameters;

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


private:
    ServerClient(void);
    std::string     _uplink;
    unsigned int    _hopcount;
    unsigned int    _token;
    std::string     _info;


};

std::ostream &   operator<<(std::ostream & o, ServerClient const & rhs);

#endif