#ifndef USER_CLASS_HPP
#define USER_CLASS_HPP
#include "Client.class.hpp"
#include <stack>
#include <unordered_map>

class Channel;

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
    User &setName(std::string const &name);
    User &setRealName(std::string const &real_name);
    User &setHostname(std::string const &hostname);
    User &setUser(std::string const &user);
    User &setOper(bool const oper);
    User &setMode(unsigned int mode);
    User &addMode(unsigned int mode);
    User &delMode(unsigned int mode);
    std::string getUser() const;
    std::unordered_map<std::string, Channel *> getChannels(void) const;
    virtual std::string getName() const;
    bool getOper() const;
    std::string getMode() const;
    void addChannel(Channel *);
    void deleteChannel(Channel *);
    User &setPassword(std::string const &pass);
    std::string getPassword() const;

protected:
    User(void);
private:
    bool _oper;
    std::string _user;
    std::string _real_name;
    std::string _hostname;
    std::string _password;
    unsigned int _mode;
    std::unordered_map<std::string, Channel *> _channelsJoined;
};

class Oper : public User
{
public:
    Oper(User *user);
    ~Oper();

    //TODO add Oper command
    void testOper();
private:
    Oper(void);
    User *_user;
};

#endif