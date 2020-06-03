#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP
#include "Client.class.hpp"
#include "ServerClient.class.hpp"
#include <stack>
#include <unordered_map>

class Channel;

class Oper
{
public:
    Oper(void);
    ~Oper();
    //TODO add Oper command
    void testOper();

private:
};

class User : public Client
{
public:
    enum Mode
    {
        a = 1,
        r = 1 << 1,
        w = 1 << 2,
        i = 1 << 3,
        o = 1 << 4,
        O = 1 << 5,
        s = 1 << 6,

    };
    User(SocketClient *socket_client);
    User(User const &instance);
    User &operator=(User const &rhs);
    ~User(void);
    User &handle() { return (*this); };
    User &setName(std::string const &name);
    User &setRealName(std::string const &real_name);
    User &setHostname(std::string const &hostname);
    User &setUser(std::string const &user);
    User &setMode(unsigned int mode);
    User &addMode(unsigned int mode);
    User &delMode(unsigned int mode);
    User &setOper(Oper &);
    std::string getUser() const;
    std::string getHostName() const;
    std::unordered_map<std::string, Channel *> getChannels(void) const;
    Channel *   getChannel(std::string const &) const;
    virtual std::string getName() const;
    std::string getRealName() const;
    Oper *getOper() const;
    std::string getMode() const;
    unsigned int getModeInt() const;
    void addChannel(Channel *);
    void deleteChannel(Channel *);
    void    sendMsgTo(User * target, std::string const & msg) const;
     
private:
    User(void);
    Oper *_oper;
    std::string _user;
    std::string _real_name;
    std::string _hostname;
    unsigned int _mode;

    std::unordered_map<std::string, Channel *> _channelsJoined;
};

std::ostream &   operator<<(std::ostream & o, User const & rhs);


#endif