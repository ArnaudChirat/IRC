#include "User.class.hpp"
#include "Channel.class.hpp"
#include "MessageMediator.class.hpp"
#include "RoutingTable.class.hpp"
#include <unordered_set>
Channel::Channel(void) {}

Channel::Channel(std::string const &name) : _name(name) {}

Channel::~Channel(void) {}

std::string Channel::getName(void) const
{
    return this->_name;
}

std::unordered_map<std::string, User *> Channel::getMembers(void) const
{
    return this->_members;
}

void Channel::addMember(User *user)
{
    this->_members.insert({user->getName(), user});
}

void Channel::deleteMember(User *user)
{
    this->_members.erase(user->getName());
}

std::string     Channel::getMembersString(char separator) const {
    std::string  str;
    for (auto it = this->_members.begin(); it != this->_members.end(); ++it)
        str += it->first + separator;
    !str.empty() ? str.pop_back() : (void)0;
    return str;
}

void Channel::sendMessageToAll(User &user, IRCMessage const &msg) const
{
    std::unordered_set<Token> used_token;
    for (auto it = this->_members.begin(); it != this->_members.end(); ++it)
    {
        Token token = IRCServer::getTokenFromUser(it->first);
        if (it->first != user.getName())
        {
            if (token == 1)
                IRCServer::_client_manager->sendMsg2(&user, msg, it->second->getName());
            else
            {
                Token road = IRCServer::_routing_table->getRoute(token);
                if (used_token.find(road) == used_token.end())
                {
                    IRCServer::_client_manager->sendMsg2(&user, msg, it->second->getName());
                    used_token.insert(road);
                }
            }
        }
    }
}

std::ostream &operator<<(std::ostream &o, Channel const &rhs)
{
    o << "channel name : " << rhs.getName() << std::endl;
    o << "Members :" << std::endl;
    o << rhs.getMembersString(',') << std::endl;
    return o;
}