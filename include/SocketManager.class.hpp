#ifndef SOCKETMANAGER_CLASS_HPP
#define SOCKETMANAGER_CLASS_HPP
#include <list>
#include <sys/socket.h>
#include "Socket.class.hpp"
#include "Client.class.hpp"
#include "Serveur.class.hpp"
class SocketManager : public SocketManagerInterface
{
public:
    SocketManager(Serveur &serveur, Socket &std_in);
    SocketManager(SocketManager const &instance) = delete;
    SocketManager &operator=(SocketManager const &rhs) = delete;
    virtual ~SocketManager(void);
    void route();
    void dispatch(Serveur &serveur);
    void dispatch(Client &client);
    void dispatch(Socket &socket);
private:
    SocketManager(void);
    void addSocket(Socket *socket);
    std::list<std::unique_ptr<Socket>> _sockets;
    fd_set _readfds, _writefds, _errorfds;
    int _max_fd;
    void setFdSet();
    bool _hasError;
};

#endif