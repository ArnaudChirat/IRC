#include "ReplyFactory.class.hpp"
#include <map>

std::string     ReplyFactory::ConnectionReplyMessage(int const x, std::vector<std::string> vstrings){
    
    std::map<int, std::string> connectionReply = {
        {ConnectionEnum::RPL_WELCOME, "Welcome to the Internet Relay Network "+vstrings[0]+"!"+vstrings[1]+"@"+vstrings[2]},
        {ConnectionEnum::RPL_YOURHOST, "Your host is <servername>, running version "+vstrings[0]},
        {ConnectionEnum::RPL_CREATED, "This server was created " +vstrings[0]},
        {ConnectionEnum::RPL_MYINFO, vstrings[0]+" "+vstrings[1]+" "+vstrings[2]+" "+vstrings[3]},
        {ConnectionEnum::RPL_BOUNCE, "Try server "+vstrings[0]+", port "+vstrings[1]},
    };
    
    return connectionReply[x];
}

std::string     ReplyFactory::CommandReplyMessage(int const x, std::vector<std::string>){
    
}

std::string     ReplyFactory::ErrorReplyMessage(int const x, std::vector<std::string>){
    
}
