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
    void    notify(Client *, std::string const &);
    void    notify(Channel *, std::string const &);

    ServerClient * getOriginOfMsg(void) const;
    void setOriginOfMsg(ServerClient*);

private:
    std::unordered_set<SocketClient*>      _subscribers;
    ServerClient * _originOfMsg;
};

#endif