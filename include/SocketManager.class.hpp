#ifndef SOCKETMANAGER_CLASS_HPP
#define SOCKETMANAGER_CLASS_HPP
#include "Socket.class.hpp"
#include <list>
class SocketManager
{
public:
    SocketManager(void);
    SocketManager(Socket *serveur);
    SocketManager(SocketManager const &instance) = delete;
    SocketManager &operator=(SocketManager const &rhs) = delete;
    ~SocketManager(void);
    void route();

private:
    void addSocket(Socket *);
    std::list<Socket *> _sockets;
    fd_set _readfds, _writefds, _errorfds;
    int _max_fd;
    void setFdSet();
};

#endif