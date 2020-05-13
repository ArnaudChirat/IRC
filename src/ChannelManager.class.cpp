#include "ChannelManager.class.hpp"
#include "IRCServer.class.hpp"
#include <iostream>
#include <algorithm>

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager(void) {}

size_t  ChannelManager::getSize(void) const{
    return this->_channels.size();
}

std::vector<std::string>  ChannelManager::_splitParam(std::string const & param, char const & delimiter) const{
    size_t idx = 0;
    size_t foundIdx = 0;
    std::vector<std::string> splitParams;
    while (idx < param.size()){
        if ((foundIdx = param.find(delimiter)) != std::string::npos){
            splitParams.
        }
    }
}

void    ChannelManager::handleJoinChannel(IRCMessage const & msg, Client * user) {
    if (!msg.getParameters().size()){
        IRCServer::_reply_manager.errorReply(user, NULL, ReplyManager::ERR_NEEDMOREPARAMS);
    }
    // decoupage des noms de channels et keys dans des vector names et keys
    std::vector<std::string>  names = _splitParam(msg.getParameters().at(0), ',');
    std::vector<std::string>  keys = (msg.getParameters().size() > 1 ? _splitParam(msg.getParameters().at(1), ',') : 0);
    // verify names. and keys? => erroreplies
    std::string channelName = msg.getParameters().at(0);
    auto it = this->_channels.find(channelName);
    if (it == this->_channels.end()){
        Channel * channel = this->_createChannel(channelName, msg.getParameters());
        this->_addChannel(channelName, channel);
        channel->addMember(user);
    }
    else
        it->second->addMember(user);
}

bool    ChannelManager::_verify(IRCMessage const & msg, Client * user) const {
    
    std::string::const_iterator it = msg.getParameters().at(0).begin();
    if (*it == '#' | *it == '&' | *it == '+' | *it == '!')
        return true;
    // A compléter        
    return false;
}

Channel *    ChannelManager::_createChannel(std::string const & name, std::vector<std::string> const & params) const{
    Channel *   newChannel = new Channel(name);
    (void)params;
    //Ici on pourra set tous les params qui seraient donnés avec le JOIN (modes...) -> a voir si 
    // ca se fait avec le Join à la creation
    return newChannel;
}

void    ChannelManager::_addChannel(std::string const & name, Channel * channel){
    this->_channels.insert({name, channel});
}


void    ChannelManager::displayChannels(void) const{
    std::cout << "Size of channel list : " << this->getSize() << std::endl; 
    std::cout << "Elements in channel list : " << std::endl; 
    for(auto it = this->_channels.begin(); it != this->_channels.end(); ++it){
        std::cout << it->first << std::endl;
        std::unordered_map<std::string, User*> members = it->second->getMembers();
        for (auto itcli = members.begin(); itcli != members.end(); ++itcli)
            std::cout << "   " << (*itcli).first << std::endl;
    }
}