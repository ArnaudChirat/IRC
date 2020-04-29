#include "IRCMessage.class.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

const std::string IRCMessage::special("\\x5B-\\x60\\x7B-\\x7D");
const std::string IRCMessage::shortname("(?:[[:alnum:]][[:alnum:]-]*[[:alnum:]]*)");
const std::string IRCMessage::hostname("(" + shortname + "(?:\\." + shortname + ")*" + ")");
const std::string IRCMessage::hostaddr("((?:[0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4}|(?:\\d{1,3}\\.){3}\\d{1,3})");
const std::string IRCMessage::host("(?:" + hostaddr + "|" + hostname + ")");
const std::string IRCMessage::nickname("([[:alpha:]" + special + "][[:alnum:]-" + special + "]{1,8})");
const std::string IRCMessage::user("([^\\x00\\x0A\\x0D\\x20\\x40]+)");
const std::string IRCMessage::prefix("^:(" + nickname + "(?:(?:!" + user + ")?@" + host + "?)|(?:" + hostname + "))");
const std::string IRCMessage::message("^\\s*(:[^ \n:]* )?([A-Za-z0-9]*)([^\n:]*)?(:.*)?");

IRCMessage::IRCMessage(std::string &message)
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
    return (*this);
}

IRCMessage &IRCMessage::setCommand(std::string const &command)
{
    this->_command = command;
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
    return (*this);
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
