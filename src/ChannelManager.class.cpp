#include "ChannelManager.class.hpp"
#include "User.class.hpp"
#include "IRCServer.class.hpp"
#include "Utility.hpp"
#include <iostream>
#include <algorithm>

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager(void) {}


void    ChannelManager::handleJoinChannel(IRCMessage const & msg, User * user) {
    if (msg.getParameters()[0] == "0")
        _leaveAllChann(user);
    else {
        std::vector<std::string>  names;
        std::vector<std::string>  keys;
        names = _splitParam(msg.getParameters().at(0), ", ");
        keys = (msg.getParameters().size() > 1 ? _splitParam(msg.getParameters().at(1), ", ") : keys);
        // verify keys?
        for (auto itName = names.begin(); itName != names.end(); ++itName)
            _createAddChannel(*itName, user);
    }
}

void    ChannelManager::handlePartChannel(IRCMessage const & msg, User * user) {
    std::vector<std::string>  names;
    std::string     partMessage;
    Channel * channel;

    names = _splitParam(msg.getParameters().at(0), ", ");
    partMessage = (msg.getParameters().size() > 1 ? msg.getParameters()[1] : partMessage);
    for (auto it = names.begin(); it != names.end(); ++it){
        if (!(channel = this->getChannel(*it))){
            Parameters param(*user);
            param.channelName = *it;
            IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOTONCHANNEL, user->getSocketClient());
        }
        else if (!(channel = user->getChannel(*it)))
            IRCServer::_reply_manager->reply(Parameters(*user).paramChannel(*channel), ReplyManager::ERR_NOTONCHANNEL, user->getSocketClient());
        else
            this->_leaveOneChann(user, channel);      
    }
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
        Parameters param(*user);
        param.channelName = name;
        IRCServer::_reply_manager->reply(param, ReplyManager::RPL_NAMREPLY, user->getSocketClient());
        return;
    }
    auto it = this->_channels.find(name);
    if (it == this->_channels.end()) {
        Channel * channel = this->_createChannel(name);
        this->_newMember(user, channel);
        this->_welcomeMessage(user, channel);
    }
    else {
        if (!user->getChannel(it->second->getName())){
            this->_newMember(user, it->second);
            this->_welcomeMessage(user, it->second);
        }
    }
}

void    ChannelManager::_welcomeMessage(User * user, Channel * channel) const{
    Parameters param = Parameters(*user).paramChannel(*channel);
    _sendParamToAll(param, channel, ReplyManager::RPL_WELCOMECHAN);
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_NAMREPLY, user->getSocketClient());
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_ENDOFNAMES, user->getSocketClient());
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

Channel *    ChannelManager::_createChannel(std::string const & name) {
    Channel *   newChannel = new Channel(name);
    this->_addChannel(name, newChannel);
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
    Parameters  param = Parameters(*user).paramChannel(*channel);
    _sendParamToAll(param, channel, ReplyManager::RPL_LEAVECHANN);
    channel->deleteMember(user);
    user->deleteChannel(channel);
}

size_t  ChannelManager::getSize(void) const{
    return this->_channels.size();
}

Channel * ChannelManager::getChannel(std::string const & name) const{
    auto it = this->_channels.find(name);
    if (it != this->_channels.end())
        return it->second;    
    return NULL;
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