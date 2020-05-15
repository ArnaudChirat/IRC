#ifndef CHANNEL_CLASS_HPP
#define CHANNEL_CLASS_HPP
#include <list>
#include <string>
#include <unordered_map>
#define STANDARD 0
#define SAFE 1

class User;

class Channel
{
public:
    Channel(void);
    Channel(std::string const &);
    Channel(Channel const &instance) = delete;
    Channel &operator=(Channel const &rhs) = delete;
    ~Channel(void);

    std::string getName(void) const;
    void    addMember(User *);
    void    deleteMember(User *);
    std::unordered_map<std::string, User*>     getMembers(void) const;
    std::string     getMembersString(void) const;

    // void addUser(User const &user);
    // void removeClient(User const &user);
    // void setClientPrivilege(unsigned int privilige, User const &user);
    // void getClientPrivilege(User const &user) const;
    // void setMode(unsigned int mode);
    // void getMode() const;

private:
    // TODO state pattern à implementer
    // Statut *statut;
    std::string  _name;
    // unsigned int _type;
    // unsigned int _modes;
    std::unordered_map<std::string, User*> _members;
};




#endif