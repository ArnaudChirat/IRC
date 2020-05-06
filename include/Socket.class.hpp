#ifndef SOCKET_CLASS_HPP
#define SOCKET_CLASS_HPP
#include <netinet/in.h>
#include <string>
class SocketServeur;
class SocketClient;
class Socket;
class SocketManagerInterface
{
public:
    virtual void dispatch(SocketServeur &serveur) = 0;
    virtual void dispatch(SocketClient &client) = 0;
    virtual void dispatch(Socket &socket) = 0;
};
class Socket
{
public:
    Socket(void);
    Socket(int const socket, sockaddr_in const &addr);
    Socket(Socket const &instance) = delete;
    Socket &operator=(Socket const &rhs) = delete;
    virtual ~Socket(void);
    int getSocket() const;
    std::string getAddr() const;
    unsigned int getPort() const;
    Socket &setAddr(sockaddr_in const &addr);
    void readStdin();
    virtual void handle(SocketManagerInterface &dispatcher);

private:
    void checkSocket() const;
    int const _socket;
    sockaddr_in _addr;
};

#endif