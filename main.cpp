
#include "Serveur.class.hpp"
#include "SocketManager.class.hpp"
#include <exception>
#include <iostream>
static const std::string word("[\\w]+");
static const std::string shortname(word + "(?:-+" + word +")*");
static const std::string hostname(shortname + R"((?:\.)" + shortname + ")*");
static const std::string servername("^(?=.{1,63}\\w$)" + hostname);
int         main() {

    try
    {
        // Serveur serveur(6667);
        // SocketManager manager(&serveur);
        // manager.route();
        std::cout << servername.c_str() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    

    return (0);
}