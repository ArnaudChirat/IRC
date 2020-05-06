#ifndef CLIENTFACTORY_CLASS_HPP
#define CLIENTFACTORY_CLASS_HPP
#include <map>
#include "Client.class.hpp"
class ClientFactory
{
public:
    enum ClientChoice
    {
        USER,
        SERVER,
        SERVICE
    };
    static Client *createClient(ClientChoice choice, SocketClient *socket_client);
};

#endif