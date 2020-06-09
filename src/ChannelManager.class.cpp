#include "ChannelManager.class.hpp"
#include "User.class.hpp"
#include "IRCServer.class.hpp"
#include "Utility.hpp"
#include "Observer.class.hpp"
#include "Channel.class.hpp"
#include "RoutingTable.class.hpp"
#include <iostream>
#include <algorithm>

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager(void) {}


void    ChannelManager::handleJoinChannel(IRCMessage const & msg, User * user, ConnectionType x) {
    Channel * channel = NULL;
    if (msg.params.channelName == "0")
        _leaveAllChann(user, ChannelManager::CommandType::JOIN);
    else {
        std::vector<std::string>  names;
        std::vector<std::string>  keys;
        names = Utility::splitParam(msg.params.channelName, ", ");
        keys = Utility::splitParam(msg.params.keys, ", ");
        // verify keys?
        for (auto itName = names.begin(); itName != names.end(); ++itName){
            channel = _createAddChannel(*itName, user, x);
            channel ? IRCServer::_observer->notify(channel, user, "JOIN") : false;
        }
    }
}

void ChannelManager::handleNJoin(IRCMessage const & message, ConnectionType x){
    std::vector<std::string>  usersNames;
    User * user = NULL;
    usersNames = Utility::splitParam(message.params.channelMembersComma, ",");
    for (auto it = usersNames.begin(); it != usersNames.end(); ++it)
    {
        user = IRCServer::getUser(*it);
        _createAddChannel(message.params.channelName, user, x);
    }
}


void    ChannelManager::handlePartChannel(IRCMessage const & msg, Client * client) {
    std::vector<std::string>  names;
    std::string     partMessage;
    Channel * channel;
    User * user = dynamic_cast<User*>(client);

    names = Utility::splitParam(msg.params.channelName, ", ");
    partMessage = msg.params.leave_message;
    for (auto it = names.begin(); it != names.end(); ++it){
        if (!(channel = this->getChannel(*it)) && user){
            Parameters param(*user);
            param.channelName = *it;
            IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOSUCHCHANNEL, user->getSocketClient());
        }
        else if (user && !(channel = user->getChannel(*it)))
            IRCServer::_reply_manager->reply(Parameters(*user).paramChannel(*channel), ReplyManager::ERR_NOTONCHANNEL, user->getSocketClient());
        else if (!user)
            user = IRCServer::getUser(msg.params.nickname);
        this->_leaveOneChann(user, channel);
        IRCServer::_observer->notify(channel, user, "PART");
    }
}

void    ChannelManager::_welcomeMessage(User * user, Channel * channel, ChannelManager::ConnectionType x) const{
    Parameters param = Parameters(*user).paramChannel(*channel);
    channel->sendParamToAll(param, ReplyManager::RPL_WELCOMECHAN);
    if (x == ConnectionType::USER){
        IRCServer::_reply_manager->reply(param, ReplyManager::RPL_NAMREPLY, user->getSocketClient());
        IRCServer::_reply_manager->reply(param, ReplyManager::RPL_ENDOFNAMES, user->getSocketClient());
    }
}

Channel *    ChannelManager::_createAddChannel(std::string name, User * user, ChannelManager::ConnectionType x) {
    Channel * channel = NULL;
    if (!_verify(name)){
        Parameters param(*user);
        param.channelName = name;
        IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOSUCHCHANNEL, user->getSocketClient());
        return channel;
    }
    channel = this->getChannel(name);
    if (!channel) {
        channel = this->_createChannel(name);
        this->_newMember(user, channel);
        this->_welcomeMessage(user, channel, x);
    }
    else
    {
        if (!user->getChannel(name))
        {
            this->_newMember(user, channel);
            this->_welcomeMessage(user, channel, x);
        }
    }
    return channel;
}

Channel *ChannelManager::_createChannel(std::string const &name)
{
    Channel *newChannel = new Channel(name);
    this->_addChannel(name, newChannel);
    return newChannel;
}

void ChannelManager::_addChannel(std::string const &name, Channel *channel)
{
    this->_channels.insert({name, channel});
}

void    ChannelManager::_leaveAllChann(User * user, ChannelManager::CommandType x) const {
    std::unordered_map<std::string, Channel*>   channs = user->getChannels();
    for (auto it = channs.begin(); it != channs.end(); ++it){
        _leaveOneChann(user, it->second);
        if (x == ChannelManager::CommandType::JOIN)
            IRCServer::_observer->notify(it->second, user, "PART");
    }

}

void ChannelManager::_leaveOneChann(User *user, Channel *channel) const
{
    Parameters param = Parameters(*user).paramChannel(*channel);
    channel->sendParamToAll(param, ReplyManager::RPL_LEAVECHANN);
    channel->deleteMember(user);
    user->deleteChannel(channel);
}

void ChannelManager::_newMember(User *user, Channel *channel)
{
    channel->addMember(user);
    user->addChannel(channel);
}

size_t ChannelManager::getSize(void) const
{
    return this->_channels.size();
}

Channel *ChannelManager::getChannel(std::string const &name) const
{
    auto it = this->_channels.find(name);
    if (it != this->_channels.end())
        return it->second;
    return NULL;
}

std::vector<Channel *> ChannelManager::getChannels() const
{
    std::vector<Channel *> channels;
    for (auto i = _channels.begin(); i != _channels.end(); i++)
    {
        channels.push_back(i->second);
    }
    return channels;
}

bool ChannelManager::_verify(std::string name) const
{
    char c = name.at(0);
    if (c == '#' | c == '&' | c == '+' | c == '!')
        return true;
    // A compléter
    return false;
}

void ChannelManager::sendMessageChannel(Client *client, Channel const &channel, IRCMessage const &msg)
{
    User *user = dynamic_cast<User *>(client);
    if (user)
    {
        if (!user->getChannel(channel.getName()))
        {
            Parameters param = Parameters(*user).paramChannel(channel);
            IRCServer::_reply_manager->reply(param, ReplyManager::ERR_NOTONCHANNEL, user->getSocketClient());
        }
        else
            channel.sendMessageToAll(*user, msg);
    }
    else
    {
        ServerClient *server = static_cast<ServerClient *>(client);
        auto users = channel.getMembers();
        for (auto i = users.begin(); i != users.end(); i++)
        {
            Token token = IRCServer::getTokenFromUser(i->second->getName());
            if (token == 1)
                IRCServer::_client_manager->sendMsg2(client, msg, i->first);
            else
            {
                Token road = IRCServer::_routing_table->getRoute(token);
                if (road != server->getToken())
                    IRCServer::_client_manager->sendMsg2(client, msg, i->first);
            }
        }
    }
}

std::vector<Channel*>  ChannelManager::getChannels(void) const{
    std::vector<Channel*> result;
    std::for_each(this->_channels.begin(), this->_channels.end(), [&result](std::pair<std::string, Channel*> x){
        result.push_back(x.second);
    });
    return result;
}