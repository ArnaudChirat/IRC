
#include "IRCServer.class.hpp"
#include <exception>
#include <iostream>
#include <unistd.h>
int main()
{

    try
    {
        IRCServer server;
        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return (0);
}