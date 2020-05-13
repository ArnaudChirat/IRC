#include "Channel.class.hpp"


Channel::Channel(void) {}

Channel::Channel(std::string const & name): _name(name) {}

Channel::~Channel(void) {}

std::string     Channel::getName(void) const{
    return this->_name;
}

std::unordered_map<std::string, User*>     Channel::getMembers(void) const{
    return this->_members;
}


void    Channel::addMember(Client * user){
    std::string name = user->getName();
    auto it = this->_members.find(name);
    if (it == this->_members.end())
        this->_members.insert({name, static_cast<User*>(user)});
}