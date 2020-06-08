#include "User.class.hpp"
#include "Channel.class.hpp"
#include "MessageMediator.class.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string const & name): _name(name) {}

Channel::~Channel(void) {}

std::string     Channel::getName(void) const{
    return this->_name;
}

std::unordered_map<std::string, User*>     Channel::getMembers(void) const{
    return this->_members;
}

void    Channel::addMember(User * user){
    this->_members.insert({user->getName(), user});
}

void    Channel::deleteMember(User * user){
    this->_members.erase(user->getName());
}

std::string     Channel::getMembersString(char separator) const {
    std::string  str;
    for (auto it = this->_members.begin(); it != this->_members.end(); ++it)
        str += it->first + separator;
    !str.empty() ? str.pop_back() : (void)0;
    return str;
}

void    Channel::sendMessageToAll(User const & user, std::string const & msg) const {
    std::string sendingmsg;
    std::string prefix = ":" + user.getName() + "!~" + user.getUser()+"@"+user.getSocketClient()->getAddr();
    sendingmsg = prefix + " PRIVMSG " + this->_name + " :" + msg + "\n";
    for (auto it = this->_members.begin(); it != this->_members.end(); ++it){
        if (it->first != user.getName())
            IRCServer::_message_mediator->sendReply(sendingmsg, it->second->getSocketClient());
    }
}

std::ostream &      operator<<(std::ostream & o, Channel const & rhs){
    o << "Channel name : " << rhs.getName() << std::endl;
    o << "Members : " << rhs.getMembersString(',') << std::endl;
    return o;
}