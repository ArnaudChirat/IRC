#include "Service.class.hpp"
#include "IRCMessage.class.hpp"
#include "IRCServer.class.hpp"
#include <regex>
Service::Service(SocketClient *socket_client) : Client(socket_client)
{
    return;
}

Service::Service(Service const &instance) : Client(instance._socket_client)
{
    this->setName(instance._name);
    return;
}
Service & Service::operator=(Service const &rhs)
{
    if (&rhs != this) {
        this->setName(rhs._name);
    }
    return (*this);
}

Service::~Service(void)
{
    return;
}

Service & Service::setName(std::string const &name)
{
    std::string real_name = name.substr(0, name.length() - IRCServer::name.length());
    std::regex e(IRCMessage::nickname.c_str());
    if (std::regex_match(real_name, e))
        this->_name = name;
    return (*this);
}
