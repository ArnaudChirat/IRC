#ifndef CHANNEL_CLASS_HPP
#define CHANNEL_CLASS_HPP
#include <list>
#include "User.class.hpp"
#include <string>
#define STANDARD 0
#define SAFE 1

class Channel
{
public:

    enum ChannelMode
    {
        ANONYMOUS = 0x01,
        INVITE_ONLY,
        MODERATED,
        NO_MESSAGE,
        QUIET,
        PRIVATE,
        SECRET,
        REOP,
        TOPIC,
        KEY,
        USER_LIMIT,
        BAN_MASK,
        EXCEPTION_MASK,
        INVITATION_MAS3,
    };
    
    Channel(void);
    // Channel(** replace parameters **);
    Channel(Channel const &instance);
    Channel &operator=(Channel const &rhs);
    ~Channel(void);
    void addUser(User const &user);
    void removeClient(User const &user);
    void setClientPrivilege(unsigned int privilige, User const &user);
    void getClientPrivilege(User const &user) const;
    void setMode(unsigned int mode);
    void getMode() const;

private:
    // TODO state pattern à implementer
    // Statut *statut;
    std::string  _name;
    unsigned int _type;
    unsigned int _modes;
    std::list<User> _members;
};




#endif