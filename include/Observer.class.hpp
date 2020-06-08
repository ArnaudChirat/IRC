#ifndef OBSERVER_CLASS_HPP
#define OBSERVER_CLASS_HPP

#include <unordered_set>
#include <string>

class SocketClient;
class ServerClient;
class Client;
class Channel;

class Observer
{
public:
    Observer(void);
    ~Observer(void);
    Observer(Observer const & instance) = delete;
    Observer &      operator=(Observer const & rhs) = delete;

    void    subscribe(SocketClient*);
    void    unsubscribe(SocketClient*);
    bool    notify(Client *, std::string const &) const;
    bool    notify(Channel *, std::string const &) const;

    SocketClient * getOriginOfMsg(void) const;
    void setOriginOfMsg(SocketClient*);

private:
    std::unordered_set<SocketClient*>      _subscribers;
    SocketClient * _originOfMsg;
};

#endif