#ifndef IRCIRCMessage_CLASS_HPP
#define IRCIRCMessage_CLASS_HPP
#include <string>
#include <vector>
#include <unordered_map>
// Représentation BNF des messages :
// message    =  [ ":" prefix SPACE ] command [ params ] crlf
// prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
// command    =  1*letter / 3digit
// params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
//            =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]

// nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
//                 ; any octet except NUL, CR, LF, " " and ":"
// middle     =  nospcrlfcl *( ":" / nospcrlfcl )
// trailing   =  *( ":" / " " / nospcrlfcl )

// SPACE      =  %x20        ; space character
// crlf       =  %x0D %x0A   ; "carriage return" "linefeed"

// target     =  nickname / server
// msgtarget  =  msgto *( "," msgto )
// msgto      =  channel / ( user [ "%" host ] "@" servername )
// msgto      =/ ( user "%" host ) / targetmask
// msgto      =/ nickname / ( nickname "!" user "@" host )
// channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
//             [ ":" chanstring ]
// servername =  hostname
// host       =  hostname / hostaddr
// hostname   =  shortname *( "." shortname )
// shortname  =  ( letter / digit ) *( letter / digit / "-" )
//             *( letter / digit )
//                 ; as specified in RFC 1123 [HNAME]
// hostaddr   =  ip4addr / ip6addr
// ip4addr    =  1*3digit "." 1*3digit "." 1*3digit "." 1*3digit
// ip6addr    =  1*hexdigit 7( ":" 1*hexdigit )
// ip6addr    =/ "0:0:0:0:0:" ( "0" / "FFFF" ) ":" ip4addr
// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
// targetmask =  ( "$" / "#" ) mask
//                 ; see details on allowed masks in section 3.3.1
// chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
// chanstring =/ %x2D-39 / %x3B-FF
//                 ; any octet except NUL, BELL, CR, LF, " ", "," and ":"
// channelid  = 5( %x41-5A / digit )   ; 5( A-Z / 0-9 )
// user       =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
//                 ; any octet except NUL, CR, LF, " " and "@"
// key        =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
//                 ; any 7-bit US_ASCII character,
//                 ; except NUL, CR, LF, FF, h/v TABs, and " "
// letter     =  %x41-5A / %x61-7A       ; A-Z / a-z
// digit      =  %x30-39                 ; 0-9
// hexdigit   =  digit / "A" / "B" / "C" / "D" / "E" / "F"
// special    =  %x5B-60 / %x7B-7D
//                 ; "[", "]", "\", "`", "_", "^", "{", "|", "}"

enum IRCMessageType
{
    CONNECTION_REGISTRATION = 0x000,
    PASS,
    NICK,
    USER,
    OPER,
    USER_MODE,
    SERVICE,
    QUIT,
    SQUIT,
    CHANNEL_OPERATION = 0x100,
    JOIN,
    PART,
    CHANNEL_MODE,
    TOPIC,
    NAMES,
    LIST,
    INVITE,
    KICK,
    SENDING_IRCMessage = 0x200,
    PRIVMSG,
    NOTICE,
    SERVER_QUERY = 0x300,
    MOTD,
    LUSERS,
    VERSION,
    STATS,
    LINKS,
    TIME,
    CONNECT,
    TRACE,
    ADMIN,
    INFO,
    SERVICE_QUERY = 0x400,
    SERVLIST,
    SQUERY,
    USER_QUERY = 0x500,
    WHO,
    WHOIS,
    WHOWAS,
    MISCELLANEOUS_IRCMessage = 0x600,
    KILL,
    PING,
    PONG,
    ERROR,
    // OPTIONAL = 0x200

    IRCMessage_TYPE_MASK = 0XFF00,
    IRCMessage_DETAIL_MASK = 0X00FF,
};
class IRCMessage
{
public:
    IRCMessage(std::string &message);
    // IRCMessage(IRCMessage const &instance);
    // IRCMessage &operator=(IRCMessage const &rhs);
    ~IRCMessage(void);
    void splitIRCMessage(std::string &message);
    IRCMessage &setPrefix(std::string const &prefix);
    IRCMessage &setCommand(std::string const &command);
    IRCMessage &setParameters(std::string const &paramaters);
    IRCMessage &setTrail(std::string const &trail);

    std::string getMessage() const;
    // std::string getPrefix() const;
    // std::string getCommand() const;
    // std::string getTrail() const;
    // const std::vector<std::string> &getParameters() const;

    static const std::string special;
    static const std::string shortname;
    static const std::string hostname;
    static const std::string hostaddr;
    static const std::string host;
    static const std::string nickname;
    static const std::string user;
    static const std::string prefix;
    static const std::string message;
    static const std::unordered_map<std::string, IRCMessageType> IRCCommands;
private:
    IRCMessage(void);
    std::string _prefix;
    std::string _command;
    std::vector<std::string> _paramaters;
    std::string _trail;
    unsigned int _type;
};


std::ostream& operator<<(std::ostream& os, const IRCMessage &message);






#endif