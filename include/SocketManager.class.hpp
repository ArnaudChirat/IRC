#ifndef SOCKETMANAGER_CLASS_HPP
#define SOCKETMANAGER_CLASS_HPP
#include <list>
#include <sys/socket.h>
#include "Socket.class.hpp"
#include "SocketClient.class.hpp"
#include "SocketServeur.class.hpp"
#include "MessageMediator.class.hpp"
class SocketManager : public SocketManagerInterface
{
public:
    SocketManager(SocketServeur &serveur, Socket &std_in);
    SocketManager(SocketManager const &instance) = delete;
    SocketManager &operator=(SocketManager const &rhs) = delete;
    virtual ~SocketManager(void);
    void route();
    void dispatch(SocketServeur &serveur);
    void dispatch(SocketClient &client);
    void dispatch(Socket &socket);
private:
    SocketManager(void);
    void addSocket(Socket *socket);
    std::list<std::unique_ptr<Socket>> _sockets;
    fd_set _readfds, _writefds, _errorfds;
    int _max_fd;
    void setFdSet();
    bool _hasError;
    MessageMediator _message_mediator;
};

#endif