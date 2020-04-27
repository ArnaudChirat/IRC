#ifndef IRCIRCMessage_CLASS_HPP
# define IRCIRCMessage_CLASS_HPP
#include <string>
#include <vector>
// La représentation BNF de ceci est :
// <IRCMessage> ::= [':' <préfixe> <espace> ] <command> <params> <crlf>
// <préfixe> ::= <nom de serveur> | <pseudo> [ '!' <utilisateur> ] [ '@' <hôte> ]
// <command> ::= <lettre> { <lettre> } | <nombre> <nombre> <nombre>
// <espace> ::= ' ' { ' ' }
// <params> ::= <espace> [ ':' <fin> | <milieu> <params> ]

// <milieu> ::= <Toute séquence non vide d'octets à l'exclusion de ESPACE, NUL, CR, LF, le premier d'entre eux étant différent de ':'>
// <fin> ::= <Toute suite, éventuellement vide, d'octets, à l'exception de NUL, CR et LF>
// <crlf> ::= CR LF
class IRCMessage
{
	public:
		IRCMessage(std::string &message);
		// IRCMessage(IRCMessage const &instance);
		// IRCMessage &operator=(IRCMessage const &rhs);
		~IRCMessage(void);
        void splitIRCMessage(std::string &message);
        // IRCMessage &setPrefix(std::string &prefix);
        // IRCMessage &setCommand(std::string &command);
        // IRCMessage &setParameters(std::string &paramaters);

        // std::string getPrefix() const;
        // std::string getCommand() const;
        // const std::vector<std::string> &getParameters() const;

	private:
		IRCMessage(void);
        std::string _prefix;
        std::string _command;
        std::vector<std::string> _paramaters;
        unsigned int _type;
};

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

#endif