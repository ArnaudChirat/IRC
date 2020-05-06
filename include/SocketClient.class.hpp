#ifndef SOCKET_CLIENT_CLASS_HPP
#define SOCKET_CLIENT_CLASS_HPP
#include <string>
#include <netinet/in.h>
#include "Socket.class.hpp"
class MessageMediator;
#include "MessageMediator.class.hpp"
class SocketClient : public Socket
{
public:
    SocketClient(int const socket, sockaddr_in const &addr);
    SocketClient(SocketClient const &instance);
    SocketClient &operator=(SocketClient const &rhs);
    ~SocketClient(void);
    bool recvMessage(MessageMediator &message_mediator);
	void handle(SocketManagerInterface &dispatcher);

private:
    SocketClient(void);
};

enum SocketClientPrivilige
{
    CREATOR = 1,
    OPERATOR = 1 << 1,
    VOICE = 1 << 2,
};

#endif