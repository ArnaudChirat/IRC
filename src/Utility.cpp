#include "Utility.hpp"
#include "IRCServer.class.hpp"
#include "IRCMessage.class.hpp"
#include "ServerClient.class.hpp"
#include "Channel.class.hpp"
#include <unordered_map>
#include <iostream>
Parameters::Parameters(void) {
}

Parameters::Parameters(Parameters const & instance) : modeint(0), hopcount(0), token(0){
    *this = instance;
}

Parameters::Parameters(Client const & client) : modeint(0), hopcount(0), token(0){
    *this = this->paramClient(client);
}

Parameters::Parameters(User const & user) : modeint(0), hopcount(0), token(0) {
    *this = this->paramUser(user);
}

Parameters::Parameters(ServerClient const & server) : modeint(0), hopcount(0), token(0) {
    *this = this->paramServer(server);
}

Parameters::Parameters(Channel const & channel) : modeint(0), hopcount(0), token(0) {
    *this = this->paramChannel(channel);
}

Parameters::Parameters(IRCMessage const & msg) : modeint(0), hopcount(0), token(0) {
    *this = this->paramMessage(msg);
}

Parameters::Parameters(IRCServer const & msg) : modeint(0), hopcount(0), token(0) {
    *this = this->paramIRCServer(msg);
}

Parameters & Parameters::paramClient(Client const & client)
{
    this->name = client.getName();
    this->host = client.getSocketClient()->getAddr();
    this->server = IRCServer::name;
    return *this;
}

Parameters & Parameters::paramUser(User const & user)
{
    this->prevNickname = user.getPrevName();
    this->nickname = user.getName();
    this->user = user.getUser();
    this->host = user.getHostName();
    this->modestr = user.getMode();
    this->real_name = user.getRealName();
    // this->server = IRCServer::name;

    return *this;
}

Parameters & Parameters::paramServer(ServerClient const & server)
{
    this->name = server.getName();
    this->uplink = server.getUplink();
    this->hopcount = server.getHopcount();
    this->token = server.getToken();
    this->serverInfo = "Other server";
    return *this;
}

Parameters & Parameters::paramChannel(Channel const & channel)
{
    this->channelName = channel.getName();
    this->channelMembers = channel.getMembersString(' ');
    this->channelMembersComma = channel.getMembersString(',');
    this->channelType = "=";
    this->server = IRCServer::name;
    return *this;
}

Parameters & Parameters::paramMessage(IRCMessage const & msg)
{
    this->command = msg.getCommand();
    return *this;
}

Parameters & Parameters::paramIRCServer(IRCServer const & server)
{
    this->name = server.name;
    this->password = server.getPassword();
    this->hopcount = 1;
    this->token = 1;
    this->serverInfo = server.info;
    return *this;
}

std::vector<std::string>  Utility::splitParam(std::string const & strToSplit, std::string const & delimiters) {
    size_t idx = 0;
    size_t foundIdx = 0;
    std::vector<std::string> splitParams;
    while (idx < strToSplit.size()){
        if ((foundIdx = strToSplit.find_first_of(delimiters, idx)) == std::string::npos){
            splitParams.push_back(strToSplit.substr(idx));
            break;
        }
        else {
            splitParams.push_back(strToSplit.substr(idx, foundIdx - idx));
            idx = foundIdx + 1;
        }
    }
    return splitParams;
}

void  *   Utility::get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET)
        return &(reinterpret_cast<sockaddr_in *>(sa)->sin_addr);
    return &(reinterpret_cast<sockaddr_in6 *>(sa)->sin6_addr);
}

bool    Utility::ipv4(struct sockaddr *sa){
    if (sa->sa_family == AF_INET)
        return true;
    return false;
}

Parameters &    Parameters::operator=(Parameters const & rhs){
    this->command = rhs.command;
    this->user = rhs.user;
    this->modeint = rhs.modeint;
    this->modestr = rhs.modestr;
    this->real_name = rhs.real_name;
    this->nickname = rhs.nickname;
    this->prevNickname = rhs.prevNickname;
    this->password = rhs.password;
    this->name = rhs.name;
    this->host = rhs.host;
    this->quit_message = rhs.quit_message;
    this->server = rhs.server;
    this->newServer = rhs.newServer;
    this->hopcount = rhs.hopcount;
    this->token = rhs.token;
    this->uplink = rhs.uplink;
    this->serverInfo = rhs.serverInfo;
    this->comment = rhs.comment;
    this->channelName = rhs.channelName;
    this->channelType = rhs.channelType;
    this->channelMembers = rhs.channelMembers;
    this->channelMembersComma = rhs.channelMembersComma;
    this->keys = rhs.keys;
    this->leave_message = rhs.leave_message;
    this->target = rhs.target;
    this->msg_target = rhs.msg_target;
    this->text = rhs.text;
    return *this;
}
