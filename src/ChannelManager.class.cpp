#include "ChannelManager.class.hpp"
#include "User.class.hpp"
#include "IRCServer.class.hpp"
#include <iostream>
#include <algorithm>

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager(void) {}

size_t  ChannelManager::getSize(void) const{
    return this->_channels.size();
}

void    ChannelManager::handleJoinChannel(IRCMessage const & msg, User * user) {
    if (!msg.getParameters().size()){
        // on fait une copie pour garder la const_ness du message initial
        IRCMessage  msgCopy = msg;
        IRCServer::_reply_manager.errorReply(&msgCopy, static_cast<Client*>(user), NULL, ReplyManager::ERR_NEEDMOREPARAMS);
        return;
    }
    if (msg.getParameters()[0] == "0"){
        _leaveAllChann(user);
        return;
    }
    std::vector<std::string>  names;
    std::vector<std::string>  keys;
    names = _splitParam(msg.getParameters().at(0), ", ");
    keys = (msg.getParameters().size() > 1 ? _splitParam(msg.getParameters().at(1), ", ") : keys);
    // verify keys?
    for (auto itName = names.begin(); itName != names.end(); ++itName)
        _createAddChannel(*itName, user);
}

std::vector<std::string>  ChannelManager::_splitParam(std::string const & param, std::string const & delimiters) const{
    size_t idx = 0;
    size_t foundIdx = 0;
    std::vector<std::string> splitParams;
    while (idx < param.size()){
        if ((foundIdx = param.find_first_of(delimiters, idx)) == std::string::npos){
            splitParams.push_back(param.substr(idx));
            break;
        }
        else {
            splitParams.push_back(param.substr(idx, foundIdx - idx));
            idx = foundIdx + 1;
        }
    }
    return splitParams;
}

void    ChannelManager::_createAddChannel(std::string name, User * user) {
    if (!_verify(name)){
        Channel *   errorChannel = new Channel(name);
        IRCServer::_reply_manager.errorReply(NULL, static_cast<Client*>(user), errorChannel, ReplyManager::ERR_NOSUCHCHANNEL);
        delete errorChannel;
        return;
    }
    auto it = this->_channels.find(name);
    if (it == this->_channels.end()) {
        Channel * channel = this->_createChannel(name, user);
        this->_welcomeMessage(user, channel);
    }
    else {
        auto itChann = user->getChannels().find(it->second->getName());
        if (itChann != user->getChannels().end()){
            this->_newMember(user, it->second);
            this->_welcomeMessage(user, it->second);
        }
    }
}

void    ChannelManager::_welcomeMessage(User * user, Channel * channel) const{
    Client * client = static_cast<Client*>(user);
    IRCServer::_reply_manager.commandReply(client, channel, ReplyManager::RPL_WELCOMECHAN);
    IRCServer::_reply_manager.commandReply(client, channel, ReplyManager::RPL_NAMREPLY);
    IRCServer::_reply_manager.commandReply(client, channel, ReplyManager::RPL_ENDOFNAMES);
}

void    ChannelManager::_newMember(User * user, Channel * channel) {
    channel->addMember(user);
    user->addChannel(channel);

}

bool    ChannelManager::_verify(std::string name) const {
    char c = name.at(0);
    if (c == '#' | c == '&' | c == '+' | c == '!')
        return true;
    // A compléter        
    return false;
}

Channel *    ChannelManager::_createChannel(std::string const & name, User * user) {
    Channel *   newChannel = new Channel(name);
    this->_addChannel(name, newChannel);
    newChannel->addMember(user);
    user->addChannel(newChannel);
    //Ici on pourra set tous les params qui seraient donnés avec le JOIN (modes...) -> a voir si 
    // ca se fait avec le Join à la creation
    return newChannel;
}

void    ChannelManager::_addChannel(std::string const & name, Channel * channel){
    this->_channels.insert({name, channel});
}

void    ChannelManager::_leaveAllChann(User * user) const {
    std::unordered_map<std::string, Channel*>   channs = user->getChannels();
    for (auto it = channs.begin(); it != channs.end(); ++it)
        _leaveOneChann(user, it->second);
}

void    ChannelManager::_leaveOneChann(User * user, Channel * channel) const {
    channel->deleteMember(user);
    user->deleteChannel(channel);
    IRCServer::_reply_manager.commandReply(static_cast<Client*>(user), channel, ReplyManager::RPL_LEAVECHANN);
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