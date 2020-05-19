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
    virtual void writeToSocket(SocketClient &) const = 0;
};
class Socket
{
public:

    enum type {
        READ = 0x01,
        WRITE,
    };

    Socket(void);
    Socket(int const socket, sockaddr const & addr);
    Socket(Socket const &instance) = delete;
    Socket &operator=(Socket const &rhs) = delete;
    virtual ~Socket(void);
    int getSocket() const;
    std::string getAddr();
    unsigned int getPort();
    void setAddr(sockaddr const & addr);
    void readStdin();
    virtual void handle(SocketManagerInterface &dispatcher, type);

private:
    void checkSocket() const;
    int const _socket;
    sockaddr _addr;
};

#endif