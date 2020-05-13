#include "User.class.hpp"
#include "IRCMessage.class.hpp"
#include <regex>
User::User(SocketClient *socket_client) : Client(socket_client), _oper(false)
{
    return;
}

User::User(User const &instance) : Client(instance._socket_client), _oper(instance._oper), _user(instance._user)
{
    this->setName(instance._name);
    return;
}
User & User::operator=(User const &rhs)
{
    if (&rhs != this) {
        this->setName(rhs._name);
        this->setUser(rhs._user);
        this->setOper(rhs._oper);
    }
    return (*this);
}

User::~User(void)
{
    return;
}

User & User::setName(std::string const &name)
{
    std::regex e(IRCMessage::nickname.c_str());
    if (std::regex_match(name, e))
        this->_name = name;
    return (*this);
}

User & User::setUser(std::string const &user)
{
    std::regex e(IRCMessage::user.c_str());
    if (std::regex_match(user, e))
        this->_user = user;
    return (*this);
}

User & User::setOper(bool const oper)
{
    //TODO set decorator for operator user
    this->_oper = oper;
    return (*this);
}

std::string User::getUser() const
{
    return (this->_user);
}

std::string User::getName() const
{
    return (this->_name);
}

bool User::getOper() const
{
    return (this->_oper);
}