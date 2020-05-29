#ifndef UTILITY_H
#define UTILITY_H
#include <netinet/in.h>
#include <string>
#include <vector>
#define BUFF_MSG 512
#define RESET "\033[0m"
#define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */

typedef unsigned int Token;
class IRCMessage;
class Channel;
class Client;
class User;
namespace Utility
{
    std::vector<std::string> splitParam(std::string const &strToSplit, std::string const &delimiters);
    void *get_in_addr(struct sockaddr *sa);
    bool ipv4(struct sockaddr *sa);
    std::string random_string(std::size_t length);
}; // namespace Utility

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
    std::string newServer;
    unsigned int hopcount;
    unsigned int token;
    std::string serverInfo;
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
    Parameters &paramClient(Client const &);
    Parameters &paramUser(User const &);
    Parameters &paramChannel(Channel const &);
    Parameters &paramMessage(IRCMessage const &);
};

#endif
