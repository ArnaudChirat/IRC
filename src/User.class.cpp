#include "User.class.hpp"
#include "Channel.class.hpp"
#include "IRCMessage.class.hpp"
#include <regex>
#include <iostream>
User::User(SocketClient *socket_client) : Client(socket_client), _oper(NULL)
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
    // if (&rhs != this)
    // {
    //     Client::operator=(rhs);
    //     this->_channelsJoined = rhs._channelsJoined;
    //     this->_hostname = rhs._hostname;
    //     this->_oper = rhs._oper;
    //     this->_user = rhs._user;
    //     this->_mode = rhs._mode;
    //     this->_real_name = rhs._real_name;
    //     this->_password = rhs._password;
    // }
    (void)rhs;
    return (*this);
}

User::User(void)
{
    return;
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

std::string User::getRealName() const{
    return this->_real_name;
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

unsigned int User::getModeIntFromStr(std::string const & modestr){
    unsigned int modeint = 0;
    for (auto it = modestr.begin(); it != modestr.end(); ++it)
    {
        switch(*it) {
            case 'a':
                modeint = modeint | Mode::a;
                break;
            case 'i':
                modeint = modeint | Mode::i;
                break;
            case 'w':
                modeint = modeint | Mode::w;
                break;
            case 'r':
                modeint = modeint | Mode::r;
                break;
            case 'o':
                modeint = modeint | Mode::o;
                break;
            case 'O':
                modeint = modeint | Mode::O;
                break;
            case 's':
                modeint = modeint | Mode::s;
                break;
        }
    }
    return modeint;
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

unsigned int User::getModeInt() const
{
    return (this->_mode);
}

User &User::setRealName(std::string const &real_name)
{
    this->_real_name = real_name;
    return (*this);
}

std::string User::getHostName(void) const{
    return this->_hostname;
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

void    User::sendMsgTo(User * target, std::string const & msg) const{
    std::string sendingmsg;
    std::string prefix = ":" + this->_name + "!~" + this->_user+"@"+this->getSocketClient()->getAddr();
    sendingmsg = prefix + " PRIVMSG " + target->getName() + " :" + msg + "\n";
    IRCServer::_message_mediator->sendReply(sendingmsg, target->getSocketClient());
}

User &User::setOper(Oper &oper)
{
    this->_oper = &oper;
    return (*this);
}

Oper *User::getOper() const
{
    return (this->_oper);
}

Oper::Oper(void)
{
    return;
}

Oper::~Oper(void)
{
    return;
}


void Oper::testOper()
{
    std::cout << "ça marche ma gueule" << std::endl;
}
Channel * User::getChannel(std::string const & name) const{
    auto it = this->_channelsJoined.find(name);
    if (it != this->_channelsJoined.end())
        return it->second;    
    return NULL;
}

std::ostream &operator<<(std::ostream &o, User const &rhs)
{
    o << "user name : " << rhs.getName() << std::endl;
    o << "real name : " << rhs.getRealName() << std::endl;
    o << "host : " << rhs.getHostName() << std::endl;
    // add channel ?
    return o;
}