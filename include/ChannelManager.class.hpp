#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include <unordered_map>
#include "IRCMessage.class.hpp"
#include "ReplyManager.class.hpp"

class Channel;
class Client;
class User;

class ChannelManager
{

public:

    enum ChannelMode
    {
        ANONYMOUS = 0x01,
        INVITE_ONLY,
        MODERATED,
        NO_MESSAGE,
        QUIET,
        PRIVATE,
        SECRET,
        REOP,
        TOPIC,
        KEY,
        USER_LIMIT,
        BAN_MASK,
        EXCEPTION_MASK,
        INVITATION_MAS3,
    };

    ChannelManager(void);
    ~ChannelManager(void);
    size_t  getSize(void) const;
    void    handleJoinChannel(IRCMessage const &, User *);
    void    handlePartChannel(IRCMessage const &, User *);
    Channel * getChannel(std::string const &) const;
    // pour debug. A virer asap
    void    displayChannels(void) const;


private:
    ChannelManager(ChannelManager const &) = delete;
    ChannelManager &    operator=(ChannelManager const &) = delete;
    
    std::unordered_map<std::string, Channel*> _channels;

    std::vector<std::string>  _splitParam(std::string const &, std::string const &) const;
    bool    _verify(std::string) const;
    Channel *    _createChannel(std::string const &);
    void    _addChannel(std::string const &, Channel *);
    void    _createAddChannel(std::string, User *);
    void    _newMember(User *, Channel *);
    void    _welcomeMessage(User * client, Channel * channel) const;
    void    _leaveAllChann(User * user) const;
    void    _leaveOneChann(User * user, Channel * channel) const;
    void    _errorNoChannelCreation(std::string const &, Client *, ReplyManager::ErrorEnum);



};

#endif