#include "ChannelManager.class.hpp"
#include "User.class.hpp"
#include "IRCServer.class.hpp"
#include "Utility.hpp"
#include "Channel.class.hpp"
#include <iostream>
#include <algorithm>

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager(void) {}


void    ChannelManager::handleJoinChannel(IRCMessage const & msg, User * user) {
    if (msg.params.channelName == "0")
        _leaveAllChann(user);
    else {
        std::vector<std::string>  names;
        std::vector<std::string>  keys;
        names = Utility::splitParam(msg.params.channelName, ", ");
        keys = Utility::splitParam(msg.params.keys, ", ");
        // verify keys?
        for (auto itName = names.begin(); itName != names.end(); ++itName)
            _createAddChannel(*itName, user);
    }
}

void    ChannelManager::handlePartChannel(IRCMessage const & msg, User * user) {
    std::vector<std::string>  names;
    std::string     partMessage;
    Channel * channel;

    names = Utility::splitParam(msg.params.channelName, ", ");
    partMessage = msg.params.leave_message;
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

void    ChannelManager::_welcomeMessage(User * user, Channel * channel) const{
    Parameters param = Parameters(*user).paramChannel(*channel);
    channel->sendParamToAll(param, ReplyManager::RPL_WELCOMECHAN);
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_NAMREPLY, user->getSocketClient());
    IRCServer::_reply_manager->reply(param, ReplyManager::RPL_ENDOFNAMES, user->getSocketClient());
}

void    ChannelManager::_createAddChannel(std::string name, User * user) {
    if (!_verify(name)){
        Parameters param(*user);
        param.channelName = name;
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOSUCHCHANNEL, user->getSocketClient());
        return;
    }
    Channel * channel = this->getChannel(name);
    if (!channel) {
        Channel * channel = this->_createChannel(name);
        this->_newMember(user, channel);
        this->_welcomeMessage(user, channel);
    }
    else {
        if (!user->getChannel(name)){
            this->_newMember(user, channel);
            this->_welcomeMessage(user, channel);
        }
    }
}

Channel *    ChannelManager::_createChannel(std::string const & name) {
    Channel *   newChannel = new Channel(name);
    this->_addChannel(name, newChannel);
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
    channel->sendParamToAll(param, ReplyManager::RPL_LEAVECHANN);
    channel->deleteMember(user);
    user->deleteChannel(channel);
}

void    ChannelManager::_newMember(User * user, Channel * channel) {
    channel->addMember(user);
    user->addChannel(channel);
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

bool    ChannelManager::_verify(std::string name) const {
    char c = name.at(0);
    if (c == '#' | c == '&' | c == '+' | c == '!')
        return true;
    // A compléter        
    return false;
}

void    ChannelManager::sendMessageChannel(User const & user, Channel const & channel, std::string const & msg){
    if (!user.getChannel(channel.getName())){
        Parameters param = Parameters(user).paramChannel(channel);
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOTONCHANNEL, user.getSocketClient());
    }
    else
        channel.sendMessageToAll(user, msg);
}
