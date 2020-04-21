
#include "Serveur.class.hpp"
#include <exception>
#include <iostream>
int         main() {

    try
    {
        Serveur serveur;
        serveur.launchServeur(6667);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    

    return (0);
}