#ifndef UTILITY_H
#define UTILITY_H
#include <netinet/in.h>
#include "Client.class.hpp"
#include <string>
#define BUFF_MSG 512
namespace Utility 
{
    int safe_dup(int fd);



    struct Parameters
    {
        std::string user;
        unsigned int mode;
        std::string real_name;
        std::string nickname;
        std::string password;
        std::string name;
        std::string quit_message;
        std::string server;
        std::string comment;
        std::string channel;
        std::string key;
        std::string target;
        std::string msg_target;
        std::string text;
    };

    Parameters Parameters_from_client(Client &);
    
}
#endif
