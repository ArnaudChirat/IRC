#ifndef OBSERVER_CLASS_HPP
#define OBSERVER_CLASS_HPP

#include <unordered_set>
#include <string>

class SocketClient;
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

private:
    std::unordered_set<SocketClient*>      _subscribers;
};

#endif