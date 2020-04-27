#include "IRCMessage.class.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
IRCMessage::IRCMessage(std::string &message)
{
    this->splitIRCMessage(message);
}

IRCMessage::~IRCMessage(void)
{
    return;
}

IRCMessage &IRCMessage::setPrefix(std::string &name)
{
    (void)name;
    return (*this);
}

void IRCMessage::splitIRCMessage(std::string &message)
{
    std::cout << "*****IRC Message split*******" << std::endl;
    std::istringstream iss(message);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
    for (std::string &part : results) {
        std::cout << part << std::endl;
    }

    std::cout << "*******IRC Message split end****" << std::endl;
}

// static const std::string letter("(?:\w)");
// static const std::string digit("(?:\d)");
// static const std::string word("[\w\d]+");
// static const std::string shortname(word + "*(?:" + word +")*");
// static const std::string hostname(shortname + "*(?:\." + shortname +")");
