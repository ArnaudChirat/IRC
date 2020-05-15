#include "Utility.hpp"
#include "IRCServer.class.hpp"
#include "IRCMessage.class.hpp"

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