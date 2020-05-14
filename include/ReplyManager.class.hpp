#ifndef REPLYMANAGER_HPP
#define REPLYMANAGER_HPP
#include <string>

#include "Client.class.hpp"
#include "User.class.hpp"
#include "Channel.class.hpp"
#include "IRCServer.class.hpp"
#include "IRCMessage.class.hpp"
#include "MessageMediator.class.hpp"
#include "Utility.hpp"

class ReplyManager
{
public:

    enum ConnectionEnum
    {
        RPL_WELCOME = 1,
        RPL_YOURHOST,
        RPL_CREATED,
        RPL_MYINFO,
        RPL_BOUNCE,
    };

    enum CommandEnum
    {
        RPL_TRACELINK = 200,
        RPL_TRACECONNECTING,
        RPL_TRACEHANDSHAKE,
        RPL_TRACEUNKNOWN,
        RPL_TRACEOPERATOR,
        RPL_TRACEUSER,
        RPL_TRACESERVER,
        RPL_TRACESERVICE,
        RPL_TRACENEWTYPE,
        RPL_TRACECLASS,
        RPL_TRACERECONNECT,
        RPL_STATSLINKINFO,
        RPL_STATSCOMMANDS,
        RPL_ENDOFSTATS = 219,
        RPL_UMODEIS = 221,
        RPL_SERVLIST = 234,
        RPL_SERVLISTEND,
        RPL_STATSUPTIME = 242,
        RPL_STATSOLINE,
        RPL_LUSERCLIENT = 251,
        RPL_LUSEROP,
        RPL_LUSERUNKNOWN,
        RPL_LUSERCHANNELS,
        RPL_LUSERME,
        RPL_ADMINME,
        RPL_ADMINLOC1,
        RPL_ADMINLOC2,
        RPL_ADMINEMAIL,
        RPL_TRACELOG = 261,
        RPL_TRACEEND,
        RPL_TRYAGAIN,
        RPL_AWAY = 301,
        RPL_USERHOST,
        RPL_ISON,
        RPL_UNAWAY = 305,
        RPL_NOWAWAY,
        RPL_WHOISUSER = 311,
        RPL_WHOISSERVER,
        RPL_WHOISOPERATOR,
        RPL_WHOWASUSER,
        RPL_ENDOFWHO,
        RPL_WHOISIDLE = 317,
        RPL_ENDOFWHOIS,
        RPL_WHOISCHANNELS,
        RPL_LISTSTART = 321,
        RPL_LIST,
        RPL_LISTEND,
        RPL_CHANNELMODEIS,
        RPL_UNIQOPIS,
        RPL_WELCOMECHAN = 330,
        RPL_NOTOPIC,
        RPL_TOPIC,
        RPL_LEAVECHANN,
        RPL_INVITING = 341,
        RPL_SUMMONING,
        RPL_INVITELIST = 346,
        RPL_ENDOFINVITELIST,
        RPL_EXCEPTLIST,
        RPL_ENDOFEXCEPTLIST,
        RPL_VERSION = 351,
        RPL_WHOREPLY,
        RPL_NAMREPLY,
        RPL_LINKS = 364,
        RPL_ENDOFLINKS,
        RPL_ENDOFNAMES,
        RPL_BANLIST,
        RPL_ENDOFBANLIST,
        RPL_ENDOFWHOWAS,
        RPL_INFO = 371,
        RPL_MOTD,
        RPL_ENDOFINFO = 374,
        RPL_MOTDSTART,
        RPL_ENDOFMOTD,
        RPL_YOUREOPER = 381,
        RPL_REHASHING,
        RPL_YOURESERVICE,
        RPL_TIME = 391,
        RPL_USERSSTART,
        RPL_USERS,
        RPL_ENDOFUSERS,
        RPL_NOUSERS,
    };

    enum ErrorEnum
    {
        ERR_NOSUCHNICK = 401,
        ERR_NOSUCHSERVER,
        ERR_NOSUCHCHANNEL,
        ERR_CANNOTSENDTOCHAN,
        ERR_TOOMANYCHANNELS,
        ERR_WASNOSUCHNICK,
        ERR_TOOMANYTARGETS,
        ERR_NOSUCHSERVICE,
        ERR_NOORIGIN,
        ERR_NORECIPIENT = 411,
        ERR_NOTEXTTOSEND,
        ERR_NOTOPLEVEL,
        ERR_WILDTOPLEVEL,
        ERR_BADMASK,
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NOMOTD,
        ERR_NOADMININFO,
        ERR_FILEERROR,
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME,
        ERR_NICKNAMEINUSE,
        ERR_NICKCOLLISION = 436,
        ERR_UNAVAILRESOURCE,
        ERR_USERNOTINCHANNEL = 441,
        ERR_NOTONCHANNEL,
        ERR_USERONCHANNEL,
        ERR_NOLOGIN,
        ERR_SUMMONDISABLED,
        ERR_USERSDISABLED,
        ERR_NOTREGISTERED = 451,
        ERR_NEEDMOREPARAMS = 461,
        ERR_ALREADYREGISTRED,
        ERR_NOPERMFORHOST,
        ERR_PASSWDMISMATCH,
        ERR_YOUREBANNEDCREEP,
        ERR_YOUWILLBEBANNED,
        ERR_KEYSET,
        ERR_CHANNELISFULL = 471,
        ERR_UNKNOWNMODE,
        ERR_INVITEONLYCHAN,
        ERR_BANNEDFROMCHAN,
        ERR_BADCHANNELKEY,
        ERR_BADCHANMASK,
        ERR_NOCHANMODES,
        ERR_BANLISTFULL,
        ERR_NOPRIVILEGES = 481,
        ERR_CHANOPRIVSNEEDED,
        ERR_CANTKILLSERVER,
        ERR_RESTRICTED,
        ERR_UNIQOPPRIVSNEEDED,
        ERR_NOOPERHOST = 491,
        ERR_UMODEUNKNOWNFLAG = 501,
        ERR_USERSDONTMATCH,
    };

    std::string replyMessage(ConnectionEnum, Parameters const &);
    std::string replyMessage(CommandEnum, Parameters const &);
    std::string replyMessage(ErrorEnum, Parameters const &);

    template< typename T >
    bool reply(Parameters const &, T const &, SocketClient *);

};

template <typename T>
bool     ReplyManager::reply(Parameters const & params, T const & enumType, SocketClient * socket){ 
    
    std::string reply = replyMessage(enumType, params);
    IRCServer::_message_mediator->sendReply(reply, socket);
    return true;
}


#endif