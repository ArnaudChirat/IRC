
#include "SocketServeur.class.hpp"
#include "SocketManager.class.hpp"
#include <exception>
#include <iostream>
#include <unistd.h>
int main()
{

    try
    {
        SocketServeur serveur(6667);
        sockaddr_in std_addr;
        Socket std_in(STDIN_FILENO, std_addr);
        SocketManager manager(serveur, std_in);
        manager.route();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return (0);
}