#ifndef SOCKET_CLIENT_CLASS_HPP
#define SOCKET_CLIENT_CLASS_HPP
#include <string>
#include <queue>
#include <netinet/in.h>
#include "Socket.class.hpp"
class SocketClient : public Socket
{
public:
    SocketClient(int const socket, sockaddr const & addr);
    SocketClient(SocketClient const &instance);
    SocketClient &operator=(SocketClient const &rhs);
    ~SocketClient(void);
    void addToQueue(std::string const & msg);
    bool recvMessage();
    bool sendMessage();
    std::string getPassword() const;
    void    setPassword(std::string const & password);
	void handle(SocketManagerInterface &dispatcher, type);

private:
    SocketClient(void);
    std::queue<std::string> _msgQueue;
    std::string             _password;
    
};

enum SocketClientPrivilige
{
    CREATOR = 1,
    OPERATOR = 1 << 1,
    VOICE = 1 << 2,
};

#endif