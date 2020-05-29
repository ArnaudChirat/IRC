#include "Utility.hpp"
#include "IRCServer.class.hpp"
#include "IRCMessage.class.hpp"
#include "Channel.class.hpp"
#include <random>
Parameters::Parameters(void) : mode(0), hopcount(0), token(0)  {}

Parameters::Parameters(Client const & client) : mode(0), hopcount(0), token(0) {
    *this = this->paramClient(client);
}

Parameters::Parameters(User const & user) : mode(0), hopcount(0), token(0) {
    *this = this->paramUser(user);
}

Parameters::Parameters(Channel const & channel) : mode(0), hopcount(0), token(0) {
    *this = this->paramChannel(channel);
}

Parameters::Parameters(IRCMessage const & msg) : mode(0), hopcount(0), token(0) {
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

std::string Utility::random_string(std::size_t length)
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}