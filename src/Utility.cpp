#include "Utility.hpp"
#include "IRCServer.class.hpp"

Utility::Parameters Utility::parametersClient(Client const & client)
{
    Utility::Parameters parameters = {};
    parameters.clientName = client.getName();
    return parameters;
}

Utility::Parameters Utility::parametersUser(User const & user)
{
    Utility::Parameters parameters = {};
    parameters.clientName = user.getName();
    parameters.user = user.getUser();
    return parameters;
}

Utility::Parameters Utility::parametersChannel(Channel const & channel)
{
    Utility::Parameters parameters = {};
    parameters.channelName = channel.getName();
    return parameters;
}

Utility::Parameters Utility::parametersUserChannel(User const & user, Channel const & channel)
{
    Utility::Parameters parameters = {};
    parameters.clientName = user.getName();
    parameters.user = user.getUser();
    parameters.server = IRCServer::name;
    parameters.channelName = channel.getName();
    parameters.channelMembers = channel.getMembersString();
    return parameters;
}