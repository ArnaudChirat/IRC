#ifndef CHANNEL_CLASS_HPP
#define CHANNEL_CLASS_HPP
#include <list>
#include "Client.class.hpp"
#include <string>
#define STANDART 0
#define SAFE 1

class Channel
{
public:
    Channel(void);
    // Channel(** replace parameters **);
    Channel(Channel const &instance);
    Channel &operator=(Channel const &rhs);
    ~Channel(void);
    void addClient(Communicator *client);
    void removeClient(Communicator const &client);
    void setClientPrivilege(unsigned int privilige);
    void getClientPrivilege(Client const &client) const;
    void setMode(unsigned int mode);
    void getMode() const;

private:
    // TODO state pattern à implementer
    // Statut *statut;
    unsigned int _type;
    unsigned int _modes;
    std::list<Client> _clients;
};

enum ChannelMode
{
    ANONYMOUS = 1,
    INVITE_ONLY = 1 << 1,
    MODERATED = 1 << 2,
    NO_MESSAGE = 1 << 3,
    QUIET = 1 << 4,
    PRIVATE = 1 << 5,
    SECRET = 1 << 6,
    REOP = 1 << 7,
    TOPIC = 1 << 8,
    KEY = 1 << 9,
    USER_LIMIT = 1 << 10,
    BAN_MASK = 1 << 11,
    EXCEPTION_MASK = 1 << 12,
    INVITATION_MASK = 1 << 13
};



#endif