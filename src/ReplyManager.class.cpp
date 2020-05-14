#include "ReplyManager.class.hpp"
#include "IRCServer.class.hpp"
#include <unordered_map>
#include <sstream>
#include <iomanip>

std::string     ReplyManager::connectionReplyMessage(ConnectionEnum x, Parameters const &) {
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> connectionReply = {
        {RPL_WELCOME, "Welcome to the Internet Relay Network "+client.nick+"!"+client.user+"@"+client.host+'\n'},
        {RPL_YOURHOST, "Your host is "+ReplyManager::serverInfo.name+", running version 1.0\n"},
    };

    return oss.str() + ' ' + connectionReply.at(x);
}

std::string     ReplyManager::commandReplyMessage(CommandEnum x, Parameters const &){
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

std::string     ReplyManager::errorReplyMessage(ErrorEnum x, Parameters const &){
    
        
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> errorReply = {
        {ERR_NONICKNAMEGIVEN, msg.cmd+" :No nickname given\n"},
        {ERR_NICKNAMEINUSE, client.nick + " :Nickname is already in use\n"},
        {ERR_NEEDMOREPARAMS, client.nick + ' ' + msg.cmd + " :Not enough parameters\n"},
        {ERR_NOSUCHCHANNEL, client.nick +' '+channel.name + " :No such channel\n"},
        {ERR_NOTONCHANNEL, client.nick +' '+channel.name + " :You are not on that channel\n"},
    };

    return oss.str() + ' ' + errorReply.at(x);
}

bool     ReplyManager::connectionReply(Parameters const &, ConnectionEnum x){ 
    
    std::string reply = commandReplyMessage(x, params);
    IRCServer::_message_mediator.sendReply(reply, socket);
    return true;
}

bool     ReplyManager::commandReply(Parameters const & params, CommandEnum x){ 

    std::string reply = commandReplyMessage(x, params);
    IRCServer::_message_mediator.sendReply(reply, socket);
    return true;
}   

bool     ReplyManager::errorReply(Parameters const & params, ErrorEnum x){ 

    std::string reply = errorReplyMessage(x, params);
    IRCServer::_message_mediator.sendReply(reply, socket);
    return true;
}   