#include "Utility.hpp"

Utility::Parameters Utility::Parameters_from_client(Client &client)
{
    Utility::Parameters parameters;
    parameters.name = client.getName();
    return parameters;
}