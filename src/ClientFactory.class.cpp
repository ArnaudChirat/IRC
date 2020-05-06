#include "ClientFactory.class.hpp"
#include "User.class.hpp"

Client *ClientFactory::createClient(ClientChoice choice, SocketClient *socket_client)
{
    if (choice == ClientChoice::USER)
        return  new User(socket_client);
    return (NULL);
}