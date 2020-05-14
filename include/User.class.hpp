#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP
#include "Client.class.hpp"
#include <stack>
#include <unordered_map>

class Channel;

class User : public Client
{
public:
    User(SocketClient *socket_client);
    User(User const &instance);
    User &operator=(User const &rhs);
    ~User(void);
    User & setName(std::string const &name);
    User & setUser(std::string const &user);
    User & setOper(bool const oper);
    std::string getUser() const;
    std::unordered_map<std::string, Channel*>   getChannels(void) const;
    virtual std::string getName() const;
    bool        getOper() const;
    void addChannel(Channel*);
    void deleteChannel(Channel*);
private:
    User(void);
    bool    _oper;
    std::string _user;
    std::unordered_map<std::string, Channel*> _channelsJoined;

};

#endif