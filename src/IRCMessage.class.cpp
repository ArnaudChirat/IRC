#include "IRCMessage.class.hpp"
#include <iostream>
#include <vector>
#include <sstream>
IRCMessage::IRCMessage(std::string &message)
{
    this->splitIRCMessage(message);
}

IRCMessage::~IRCMessage(void)
{
    return;
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
