#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP
#include "Client.class.hpp"
#include <stack>
class User : public Client
{
public:
    User(SocketClient *socket_client);
    User(User const &instance);
    User &operator=(User const &rhs);
    ~User(void);
    User &setName(std::string const &name);
    User &setUser(std::string const &user);
    User &setOper(bool const oper);
    std::string getUser() const;
    bool        getOper() const;
private:
    User(void);
    bool    _oper;
    std::string _user;
};

#endif