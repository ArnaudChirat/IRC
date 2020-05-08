#ifndef SOCKET_CLIENT_CLASS_HPP
#define SOCKET_CLIENT_CLASS_HPP
#include <string>
#include <netinet/in.h>
#include "Socket.class.hpp"
class SocketClient : public Socket
{
public:
    SocketClient(int const socket, sockaddr_in const &addr);
    SocketClient(SocketClient const &instance);
    SocketClient &operator=(SocketClient const &rhs);
    ~SocketClient(void);
    void appendToBuffer(std::string const & msg);
    bool recvMessage();
    bool sendMessage();
	void handle(SocketManagerInterface &dispatcher, type);

private:
    SocketClient(void);
    std::string     _buffer;
};

enum SocketClientPrivilige
{
    CREATOR = 1,
    OPERATOR = 1 << 1,
    VOICE = 1 << 2,
};

#endif