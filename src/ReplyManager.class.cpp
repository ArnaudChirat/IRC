#include "ReplyManager.class.hpp"
#include "IRCServer.class.hpp"
#include <unordered_map>
#include <sstream>
#include <iomanip>

ReplyManager::t_serverInfo  ReplyManager::serverInfo = {.name = IRCServer::name};

std::string     ReplyManager::connectionReplyMessage(ConnectionEnum x, ReplyManager::t_clientInfo client) {
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> connectionReply = {
        {RPL_WELCOME, "Welcome to the Internet Relay Network "+client.nick+"!"+client.user+"@"+client.host+'\n'},
        {RPL_YOURHOST, "Your host is "+ReplyManager::serverInfo.name+", running version 1.0\n"},
    };

    return oss.str() + ' ' + connectionReply.at(x);
}

std::string     ReplyManager::commandReplyMessage(CommandEnum x, ReplyManager::t_clientInfo client,
                                                ReplyManager::t_channelInfo channel){
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> commandReply = {
        {RPL_WELCOMECHAN, ":"+client.nick+"!~"+client.user+'@'+client.host+" JOIN :"+channel.name+"\n"},
        {RPL_NAMREPLY, oss.str()+' '+client.nick+' '+channel.type+" "+channel.name+" :"+channel.members+"\n"},
        {RPL_ENDOFNAMES, oss.str()+' '+channel.name+" :End of NAMES list\n"},
        {RPL_LEAVECHANN, ":"+client.nick+"!~"+client.user+'@'+client.host+" PART :"+channel.name+"\n"},
    };

    return commandReply.at(x);
}

std::string     ReplyManager::errorReplyMessage(ErrorEnum x, ReplyManager::t_msgInfo msg,
                                                ReplyManager::t_clientInfo client,
                                                ReplyManager::t_channelInfo channel){
    
        
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> errorReply = {
        {ERR_NONICKNAMEGIVEN, ":No nickname given\n"},
        {ERR_NICKNAMEINUSE, client.nick + " :Nickname is already in use\n"},
        {ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)\n"},
        {ERR_ERRONEUSNICKNAME, client.nick + " :Erroneous nickname\n"},
        {ERR_NEEDMOREPARAMS, client.nick + ' ' + msg.cmd + " :Not enough parameters\n"},
        {ERR_NOSUCHCHANNEL, channel.name + " :No such channel\n"},
    };

    return oss.str() + ' ' + errorReply.at(x);
}

bool     ReplyManager::connectionReply(Client * client, ConnectionEnum x){ 
    ReplyManager::t_clientInfo clientInfo = {};
    _buildClientInfo(client, clientInfo);
    IRCServer::_message_mediator.sendReply(connectionReplyMessage(x, clientInfo), client);
    return true;
}

bool     ReplyManager::commandReply(Client * client, Channel * channel, CommandEnum x){ 

    ReplyManager::t_msgInfo msgInfo = {};
    ReplyManager::t_clientInfo clientInfo = {};
    ReplyManager::t_channelInfo channelInfo = {};
    _buildClientInfo(client, clientInfo);
    _buildChannelInfo(channel, channelInfo);

    IRCServer::_message_mediator.sendReply(commandReplyMessage(x, clientInfo, channelInfo), client);
    return true;
}   

bool     ReplyManager::errorReply(IRCMessage * msg, Client * client, Channel * channel, ErrorEnum x){ 

    ReplyManager::t_msgInfo msgInfo = {};
    ReplyManager::t_clientInfo clientInfo = {};
    ReplyManager::t_channelInfo channelInfo = {};
    _buildClientInfo(client, clientInfo);
    _buildChannelInfo(channel, channelInfo);
    _buildMsgInfo(msg, msgInfo);

    IRCServer::_message_mediator.sendReply(errorReplyMessage(x, msgInfo, clientInfo, channelInfo), client);
    return true;
}   

void    ReplyManager::_buildMsgInfo(IRCMessage * msg, ReplyManager::t_msgInfo & msgInfo) {
    if (msg) {
        msgInfo.cmd = msg->getCommand();
    }
}

void    ReplyManager::_buildClientInfo(Client * client, ReplyManager::t_clientInfo & clientInfo) {
    if (client) {
        clientInfo.nick = client->getName();
        clientInfo.user = dynamic_cast<User*>(client)->getUser();
        clientInfo.host = client->getSocketClient()->getAddr();
    }
}

void    ReplyManager::_buildChannelInfo(Channel * channel, ReplyManager::t_channelInfo & channelInfo) {
    if (channel) {
        std::ostringstream  chanOSS;
        std::unordered_map<std::string, User*> members = channel->getMembers();
        for (auto it = members.begin(); it != members.end(); ++it)
            chanOSS << it->first << ' ';

        channelInfo.name = channel->getName();
        channelInfo.members = chanOSS.str();
        channelInfo.type = '='; //@ for secret and * for private
    }
}
