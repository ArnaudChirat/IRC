#ifndef CLIENT_CLASS_HPP
#define CLIENT_CLASS_HPP
#include <string>
#include <netinet/in.h>
#include "Socket.class.hpp"
#include "IClientState.class.hpp"
class Client : public Socket
{
public:
    Client(int const socket, sockaddr_in const &addr);
    Client(Client const &instance);
    Client &operator=(Client const &rhs);
    ~Client(void);
    bool recvMessage();
	void handle(SocketManagerInterface &dispatcher);
    std::string getNick() const;
    std::string getUser() const;
    Client &setState(IClientState *state);

private:
    Client(void);
    IClientState *_state;
    unsigned int _privilege;
    std::string _nick;
    std::string _user;
};

enum ClientPrivilige
{
    CREATOR = 1,
    OPERATOR = 1 << 1,
    VOICE = 1 << 2,
};

#endif