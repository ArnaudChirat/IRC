#include "ChannelManager.class.hpp"
#include "IRCServer.class.hpp"
#include <iostream>
#include <algorithm>

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager(void) {}

size_t  ChannelManager::getSize(void) const{
    return this->_channels.size();
}


bool    ChannelManager::verify(IRCMessage const & msg, Client * user) const {
    if (!msg.getParameters().size()){
        IRCServer::_reply_manager.errorReply(user, NULL, ReplyManager::ERR_NEEDMOREPARAMS);
    }
    else {
        std::string::const_iterator it = msg.getParameters().at(0).begin();
        if (*it == '#' | *it == '&' | *it == '+' | *it == '!')
            return true;
        // A compléter        
    }
    return false;
}

void    ChannelManager::handleJoinChannel(IRCMessage const & msg, Client * user) {
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
        for (auto itcli = it->second->getMembers().begin(); itcli != it->second->getMembers().end(); ++itcli)
            std::cout << "   " << itcli->first << std::endl;
    }
}