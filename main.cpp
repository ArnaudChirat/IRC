
#include "Serveur.class.hpp"
#include "SocketManager.class.hpp"
#include <exception>
#include <iostream>

int main()
{

    try
    {
        Serveur serveur(6667);
        SocketManager manager(&serveur);
        manager.route();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return (0);
}