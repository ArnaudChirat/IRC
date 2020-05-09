#include "IRCMessage.class.hpp"
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

const std::unordered_map<std::string, IRCMessageType> IRCMessage::IRCCommands{
    {"PASS", IRCMessageType::PASS},
    {"NICK", IRCMessageType::NICK},
    {"USER", IRCMessageType::USER},
    {"QUIT", IRCMessageType::QUIT},
};

IRCMessage::IRCMessage(std::string &message) : _is_valid(false)
{
    this->splitIRCMessage(message);
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
        this->_is_valid = true;
    }
    return (*this);
}

IRCMessage &IRCMessage::setParameters(std::string const &parameters)
{
    std::stringstream ss(parameters);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);
    this->_paramaters = vstrings;
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
    message += "Parameters :" + (this->_paramaters.empty() ? "EMPTY" : std::string("")) + "\n";
    for (std::string const &part : this->_paramaters)
    {
        message += "\t- " + part + "\n";
    }
    message += "Trail : " + (this->_trail.empty() ? "EMPTY" : this->_trail);
    return message;
}

bool IRCMessage::isValid() const
{
    return (this->_is_valid);
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

const std::vector<std::string> &IRCMessage::getParameters() const {
    return (this->_paramaters);
};

std::ostream &operator<<(std::ostream &os, const IRCMessage &message)
{
    os << message.getMessage();
    return os;
}

std::ostream &operator<<(std::ostream &os, const IRCMessageType &type)
{
    switch (type)
    {
    case IRCMessageType::NICK:
        os << "NICK COMMAND";
        break;
    case IRCMessageType::USER:
        os << "USER COMMAND";
        break;
    default:
        os << "UNKNOW";
        break;
    }
    return os;
}