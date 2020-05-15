#include "Utility.hpp"
#include "IRCServer.class.hpp"
#include "IRCMessage.class.hpp"
#include "Channel.class.hpp"

Parameters::Parameters(void) {}

Parameters::Parameters(Client const & client) {
    *this = this->paramClient(client);
}

Parameters::Parameters(User const & user) {
    *this = this->paramUser(user);
}

Parameters::Parameters(Channel const & channel) {
    *this = this->paramChannel(channel);
}

Parameters::Parameters(IRCMessage const & msg) {
    *this = this->paramMessage(msg);
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
    this->nickname = user.getName();
    this->user = user.getUser();
    this->host = user.getSocketClient()->getAddr();
    this->server = IRCServer::name;
    return *this;
}

Parameters & Parameters::paramChannel(Channel const & channel)
{
    this->channelName = channel.getName();
    this->channelMembers = channel.getMembersString();
    this->channelType = "=";
    this->server = IRCServer::name;
    return *this;
}

Parameters & Parameters::paramMessage(IRCMessage const & msg)
{
    this->command = msg.getCommand();
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