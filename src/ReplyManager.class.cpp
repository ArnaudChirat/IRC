#include "ReplyManager.class.hpp"
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <iostream>


std::string     ReplyManager::replyMessage(ConnectionEnum x, Parameters const & params) {
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> connectionReply = {
        {RPL_WELCOME, "Welcome to the Internet Relay Network "+params.nickname+"!"+params.user+"@"+params.host+'\n'},
        {RPL_YOURHOST, "Your host is "+params.server+", running version 1.0\n"},
    };

    return oss.str() + ' ' + connectionReply.at(x);
}

std::string     ReplyManager::replyMessage(CommandEnum x, Parameters const & params){
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> commandReply = {

        {RPL_WELCOMECHAN, ":"+params.nickname+"!~"+params.user+'@'+params.host+" JOIN :"+params.channelName+"\n"},
        {RPL_NAMREPLY, oss.str()+' '+params.nickname+' '+params.channelType+" "+params.channelName+" :"+params.channelMembers+"\n"},
        {RPL_ENDOFNAMES, oss.str()+' '+params.channelName+" :End of NAMES list\n"},
        {RPL_LEAVECHANN, ":"+params.nickname+"!~"+params.user+'@'+params.host+" PART :"+params.channelName+"\n"},
        {RPL_YOUREOPER, ":You are now an IRC operator\n"},
    };

    return commandReply.at(x);
}

std::string     ReplyManager::replyMessage(ErrorEnum x, Parameters const & params){
    
        
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> errorReply = {
        {ERR_NONICKNAMEGIVEN, params.command+" :No nickname given\n"},
        {ERR_NICKNAMEINUSE, params.nickname + " :Nickname is already in use\n"},
        {ERR_NEEDMOREPARAMS, params.nickname + ' ' + params.command + " :Not enough parameters\n"},
        {ERR_NOSUCHCHANNEL, params.nickname +' '+params.channelName + " :No such channel\n"},
        {ERR_NOTONCHANNEL, params.nickname +' '+params.channelName + " :You are not on that channel\n"},
    };

    return oss.str() + ' ' + errorReply.at(x);
}
