#ifndef CHANNEL_CLASS_HPP
#define CHANNEL_CLASS_HPP
#include <list>
#include <string>
#include <unordered_map>
#include "IRCServer.class.hpp"
#include "ReplyManager.class.hpp"

#define STANDARD 0
#define SAFE 1

class User;
struct Parameters;

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
    
    template< typename T >
    void    sendParamToAll(Parameters const &, T const &) const;

private:
    std::string  _name;
    std::unordered_map<std::string, User*> _members;

};


template <typename T>
void     Channel::sendParamToAll(Parameters const & param, T const & replyEnum) const
{
    for (auto it = this->_members.begin(); it != this->_members.end(); ++it)
        IRCServer::_reply_manager->reply(param, replyEnum, it->second->getSocketClient());
}


#endif