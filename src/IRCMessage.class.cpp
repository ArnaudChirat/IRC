#include "IRCMessage.class.hpp"
#include "IRCServer.class.hpp"
#include "Utility.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

const std::string IRCMessage::special("\\x5B-\\x60\\x7B-\\x7D");
const std::string IRCMessage::shortname("(?:[[:alnum:]][[:alnum:]-]*[[:alnum:]]*)");
const std::string IRCMessage::hostname("(" + shortname + "(?:\\." + shortname + ")*" + ")");
const std::string IRCMessage::hostaddr("((?:[0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4}|(?:\\d{1,3}\\.){3}\\d{1,3})");
const std::string IRCMessage::host("(?:" + hostaddr + "|" + hostname + ")");
const std::string IRCMessage::nickname("([[:alpha:]" + special + "][[:alnum:]-" + special + "]{0,8})");
const std::string IRCMessage::user("([^\\x00\\x0A\\x0D\\x20\\x40]+)");
const std::string IRCMessage::prefix("^:(" + nickname + "(?:(?:!" + user + ")?@" + host + "?)|(?:" + hostname + "))");
const std::string IRCMessage::message("^\\s*(:[^ \n:]* )?([A-Za-z0-9]*)([^\n:]*)?(:.*)?");

const std::unordered_map<std::string, IRCMessage::IRCMessageType> IRCMessage::IRCCommands{
    {"PASS", PASS},
    {"NICK", NICK},
    {"USER", USER},
    {"QUIT", QUIT},
    {"JOIN", JOIN},
    {"PART", PART},
};

IRCMessage::IRCMessage(std::string &message, SocketClient * socket) : _is_valid(true), _socket(socket)
{
    this->splitIRCMessage(message);
}

IRCMessage::IRCMessage(IRCMessage const &instance)
{
    *this = instance;
}

IRCMessage &    IRCMessage::operator=(IRCMessage const & rhs){
    this->_prefix = rhs.getPrefix();
    this->_command = rhs.getCommand();
    this->_parameters = rhs.getParameters();
    this->_trail = rhs.getTrail();
    return *this;
}

IRCMessage::~IRCMessage(void)
{
    return;
}

IRCMessage &IRCMessage::setPrefix(std::string const &prefix)
{
    this->_prefix = prefix;
    this->_prefix.erase(0, 1);
    return (*this);
}

IRCMessage &IRCMessage::setCommand(std::string const &command)
{
    std::string command_rec = command;
    std::transform(command_rec.begin(), command_rec.end(), command_rec.begin(), ::toupper);
    auto res = IRCMessage::IRCCommands.find(command_rec);
    if (res != IRCMessage::IRCCommands.end())
    {
        this->_command = res->first;
        this->type = res->second;
    }
    return (*this);
}

IRCMessage &IRCMessage::setParameters(std::string const &parameters)
{
    std::stringstream ss(parameters);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);
    this->_parameters = vstrings;
    return (*this);
}

IRCMessage &IRCMessage::setTrail(std::string const &trail)
{
    this->_trail = trail;
    this->_trail.erase(0, 1);
    return (*this);
}

std::string IRCMessage::getMessage() const
{
    std::string message;
    message += "Prefix : " + (this->_prefix.empty() ? "EMPTY" : this->_prefix) + "\n";
    message += "Command : " + (this->_command.empty() ? "EMPTY" : this->_command) + "\n";
    message += "Parameters :" + (this->_parameters.empty() ? "EMPTY" : std::string("")) + "\n";
    for (std::string const &part : this->_parameters)
    {
        message += "\t- " + part + "\n";
    }
    message += "Trail : " + (this->_trail.empty() ? "EMPTY" : this->_trail);
    return message;
}

bool IRCMessage::isValid()
{
    bool validation = false;
    if (this->type == USER && _parameters.size() >= 3)
    {
        parameters_struct.user = _parameters[0];
        parameters_struct.mode = std::stoi(_parameters[1]);
        parameters_struct.real_name = _trail;
        validation = true;
    }
    else if (this->type == NICK && _parameters.size() >= 1)
    {
        parameters_struct.nickname = _parameters[0];
        validation = true;
    }
    // else
        // IRCServer::_reply_manager.errorReply(Parameters(*this), ReplyManager::ERR_NEEDMOREPARAMS);

    return (validation);
}

/*
regex pattern split message :
cm[0] = full message
cm[1] = prefix
cm[2] = command
cm[3] = param
cm[4] = trail
*/
void IRCMessage::splitIRCMessage(std::string &message)
{
    std::regex e(IRCMessage::message.c_str());
    std::cmatch cm;
    bool res = std::regex_match(message.c_str(), cm, e);
    if (res)
        setPrefix(cm[1]).setCommand(cm[2]).setParameters(cm[3]).setTrail(cm[4]);
}

std::string     IRCMessage::getCommand() const{
    return this->_command;
}

std::string  IRCMessage::getPrefix() const{
    return this->_prefix;
}

std::string     IRCMessage::getTrail() const{
    return this->_trail;
}

std::vector<std::string>  IRCMessage::getParameters() const {
    return (this->_parameters);
};

std::ostream &operator<<(std::ostream &os, const IRCMessage &message)
{
    os << message.getMessage();
    return os;
}

std::ostream &operator<<(std::ostream &os, const IRCMessage::IRCMessageType &type)
{
    switch (type)
    {
    case IRCMessage::IRCMessageType::NICK:
        os << "NICK COMMAND";
        break;
    case IRCMessage::IRCMessageType::USER:
        os << "USER COMMAND";
        break;
    default:
        os << "UNKNOW";
        break;
    }
    return os;
}