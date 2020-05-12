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
        // {ConnectionEnum::RPL_CREATED, "This server was created " +vstrings[0]},
        // {ConnectionEnum::RPL_MYINFO, vstrings[0]+" "+vstrings[1]+" "+vstrings[2]+" "+vstrings[3]},
        // {ConnectionEnum::RPL_BOUNCE, "Try server "+vstrings[0]+", port "+vstrings[1]},
    };

    return oss.str() + ' ' + connectionReply.at(x);
}

// std::string     ReplyManager::commandReplyMessage(CommandEnum, std::vector<std::string>){
    
// }

std::string     ReplyManager::errorReplyMessage(ErrorEnum x, ReplyManager::t_clientInfo client,
                                                ReplyManager::t_channelInfo channel){
    
        
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << x;

    std::unordered_map<int, std::string> errorReply = {
        {ERR_NONICKNAMEGIVEN, ":No nickname given\n"},
        {ERR_NICKNAMEINUSE, client.nick + " :Nickname is already in use\n"},
    };
    (void)channel;

    return oss.str() + ' ' + errorReply.at(x);
}

bool     ReplyManager::connectionReply(Client * client, ConnectionEnum x){ 

    ReplyManager::t_clientInfo clientInfo = {};
    if (client) {
        clientInfo.nick = client->getName();
        clientInfo.user = dynamic_cast<User*>(client)->getUser();
        clientInfo.host = client->getSocketClient()->getAddr();
    }
    IRCServer::_message_mediator.sendReply(connectionReplyMessage(x, clientInfo), client);
    return true;
}

bool     ReplyManager::errorReply(Client * client, Channel * channel, ErrorEnum x){ 

    ReplyManager::t_clientInfo clientInfo = {};
    ReplyManager::t_channelInfo channelInfo = {};
    if (client) {
        clientInfo.nick = client->getName();
        clientInfo.user = "getter user de client";
        clientInfo.host = client->getSocketClient()->getAddr();
    }
    if (channel) {
        channelInfo.name = "getter de channel name";
    }

    // A voir pour les messages à envoyer à toutes les personnes d'un channel...?
    // Envoyer client, channel, puis un flag 1 = client, 2 = channel... et le message
    //mediator gère la transmission?

    IRCServer::_message_mediator.sendReply(errorReplyMessage(x, clientInfo, channelInfo), client);
    return true;
}   

