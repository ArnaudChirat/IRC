#include "User.class.hpp"
#include "Channel.class.hpp"
#include "IRCMessage.class.hpp"
#include <regex>
#include <iostream>
User::User(SocketClient *socket_client) : Client(socket_client), _oper(false)
{
    return;
}

User::User(User const &instance) : Client(instance._socket_client), _oper(instance._oper), _user(instance._user)
{
    this->setName(instance._name);
    return;
}
User &User::operator=(User const &rhs)
{
    if (&rhs != this)
    {
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

User &User::setName(std::string const &name)
{
    std::regex e(IRCMessage::nickname.c_str());
    if (std::regex_match(name, e))
        this->_name = name;
    return (*this);
}

User &User::setUser(std::string const &user)
{
    std::regex e(IRCMessage::user.c_str());
    if (std::regex_match(user, e))
        this->_user = user;
    return (*this);
}

std::string User::getUser() const
{
    return (this->_user);
}

User &User::setOper(bool const oper)
{
    //TODO set decorator for operator user
    this->_oper = oper;
    return (*this);
}

bool User::getOper() const
{
    return (this->_oper);
}

User &User::setMode(unsigned int mode)
{
    this->_mode = mode;
    return (*this);
}

User &User::addMode(unsigned int mode)
{
    this->_mode |= mode;
    return (*this);
}

User &User::delMode(unsigned int mode)
{
    this->_mode &= ~mode;
    return (*this);
}

std::string User::getMode() const
{
    std::string modes;
    if (this->_mode & Mode::a)
        modes += 'a';
    if (this->_mode & Mode::i)
        modes += 'i';
    if (this->_mode & Mode::w)
        modes += 'w';
    if (this->_mode & Mode::r)
        modes += 'r';
    if (this->_mode & Mode::o)
        modes += 'o';
    if (this->_mode & Mode::O)
        modes += 'o';
    if (this->_mode & Mode::s)
        modes += 'o';
    return modes;
}

User &User::setRealName(std::string const &real_name)
{
    this->_real_name = real_name;
    return (*this);
}

User &User::setHostname(std::string const &hostname)
{
    this->_hostname = hostname;
    return (*this);
}

std::string User::getName() const
{
    return (this->_name);
}

std::unordered_map<std::string, Channel*>  User::getChannels(void) const{
    return this->_channelsJoined;
}


void User::addChannel(Channel * channel){
    this->_channelsJoined.insert({channel->getName(), channel});
}

void User::deleteChannel(Channel * channel){
    this->_channelsJoined.erase(channel->getName());
}
