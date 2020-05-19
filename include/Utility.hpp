#ifndef UTILITY_H
#define UTILITY_H
#include <netinet/in.h>
#include "Client.class.hpp"
#include "User.class.hpp"
#include <string>
#include <vector>
#define BUFF_MSG 512

class IRCMessage;
class Channel;

namespace Utility {
    std::vector<std::string>  splitParam(std::string const & strToSplit, std::string const & delimiters);
    void    *get_in_addr(struct sockaddr *sa);
    bool    ipv4(struct sockaddr *sa);

};

struct Parameters
    {
        std::string command;
        std::string user;
        unsigned int mode;
        std::string real_name;
        std::string nickname;
        std::string password;
        std::string name;
        std::string host;
        std::string quit_message;
        std::string server;
        std::string comment;
        std::string channelName;
        std::string channelType;
        std::string channelMembers;
        std::string keys;
        std::string leave_message;
        std::string target;
        std::string msg_target;
        std::string text;

        Parameters(void);
        Parameters(Client const &);
        Parameters(User const &);
        Parameters(Channel const &);
        Parameters(IRCMessage const &);
        Parameters & paramClient(Client const &);
        Parameters & paramUser(User const &);
        Parameters & paramChannel(Channel const &);
        Parameters & paramMessage(IRCMessage const &);
    };

#endif
