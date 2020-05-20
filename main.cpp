
#include "IRCServer.class.hpp"
#include <exception>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <exception>

void    errorArguments(void){
    throw std::logic_error("[Usage]: ./server [host:port_network] <port> <password>");
}

std::vector<std::string> parseNetwork(std::string info){
    std::vector<std::string> networkInfo;
    size_t x;
    if ((x = info.find(":")) == std::string::npos || x == (info.size() - 1))
        errorArguments();
    std::string hostNetwork = info.substr(0, x);
    std::string portNetwork = info.substr(x + 1, std::string::npos);
    networkInfo.push_back(hostNetwork);
    networkInfo.push_back(portNetwork);
    if (networkInfo.size() != 2)
        errorArguments();
    return networkInfo;
}

void    launchServer(unsigned short const port, std::string const password, std::vector<std::string> networkInfo)
{
    try
    {
        std::string hostNetwork;
        std::string portNetwork;
        IRCServer server;
        server.config(port, password);
        if (!networkInfo.empty()){
            hostNetwork = networkInfo[0];
            portNetwork = networkInfo[1];
            server.connectNetwork(hostNetwork, portNetwork);
            server.joinIRCNetwork();
        }
        server.run();
    }
    catch (const std::exception &e)
    {   
        std::cerr << e.what() << std::endl;
    }
}

int main(int argc, char **argv)
{
    try {
        if (argc >= 3){
            int const port = (argc == 3 ? std::stoi(argv[1]) : std::stoi(argv[2]));
            std::string const password = (argc == 3 ? argv[2] : argv[3]);
            std::vector<std::string> networkInfo;
            // checker erreur de format des infos? ou sera catché par le try catch?
            networkInfo = (argc > 3 ? parseNetwork(argv[1]) : networkInfo);
            launchServer(port, password, networkInfo);
        }
        else {
            errorArguments();
        }
    } catch(std::exception const & e) {
        std::cerr << e.what() << std::endl;
    }

    return (0);
}