#ifndef SOCKETMANAGER_CLASS_HPP
#define SOCKETMANAGER_CLASS_HPP
#include <list>
#include <sys/socket.h>
#include <memory>
#include "Socket.class.hpp"
#include "SocketClient.class.hpp"
#include "SocketServeur.class.hpp"
#include "MessageMediator.class.hpp"
class SocketManager : public SocketManagerInterface
{
public:
    SocketManager(void);
    // SocketManager(SocketServeur &serveur, Socket &std_in);
    SocketManager(SocketManager const &instance);
    SocketManager &operator=(SocketManager const &rhs);
    virtual ~SocketManager(void);
    bool route();
    virtual void dispatch(SocketServeur &serveur);
    virtual void dispatch(SocketClient &client);
    virtual void dispatch(Socket &socket);
    virtual void writeToSocket(SocketClient &) const;
    void addSocket(Socket *socket);
    void deleteSocket(Socket *socket);
private:
    typedef std::list<std::unique_ptr<Socket>>::iterator pos;
    std::list<std::unique_ptr<Socket>> _sockets;
    fd_set _readfds, _writefds, _errorfds;
    bool _running;
    int _max_fd;
    void setFdSet();
    bool _hasError;
};

#endif