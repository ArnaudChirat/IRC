#include "IRCMessage.class.hpp"
#include "IRCServer.class.hpp"
#include "ReplyManager.class.hpp"
#include "SocketClient.class.hpp"
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
    {"PASS", IRCMessageType::PASS},
    {"NICK", IRCMessageType::NICK},
    {"USER", IRCMessageType::USER},
    {"QUIT", IRCMessageType::QUIT},
    {"SERVICE", IRCMessageType::SERVICE},
    {"JOIN", IRCMessageType::JOIN},
    {"OPER", IRCMessageType::OPER},
    {"PART", IRCMessageType::PART},
    {"PRIVMSG", IRCMessageType::PRIVMSG},
    {"LUSERS", IRCMessageType::LUSERS},
    {"SERVER", IRCMessageType::SERVER},
};

IRCMessage::IRCMessage(void)
{
}

IRCMessage::IRCMessage(Parameters const & param, std::string const & command){
    std::vector<std::string> parameters;
    this->setCommand(command);
    if (command == "NICK"){
        parameters.push_back(param.nickname);
        if(param.prevNickname.empty()){
            parameters.push_back(std::to_string(param.hopcount));
            parameters.push_back(param.user);
            parameters.push_back(param.host);
            parameters.push_back(std::to_string(param.token));
            parameters.push_back(param.modestr);
            this->setTrail(param.real_name, IRCMessageWay::SENDING);
        } else {
            this->setPrefix(param.prevNickname, IRCMessageWay::SENDING);
        }
        this->setParameters(parameters);
    }
    else if (command == "PASS"){
        // this->setPrefix(":" + ownServer.name, IRCMessageWay::SENDING);
        this->setParameters(param.password);
    }
    else if (command == "SERVER"){
        this->setPrefix(param.uplink, IRCMessageWay::SENDING);
        parameters.push_back(param.name);
        parameters.push_back(std::to_string(param.hopcount));
        parameters.push_back(std::to_string(param.token));
        this->setParameters(parameters);
        this->setTrail(param.serverInfo, IRCMessageWay::SENDING);
    }
}


IRCMessage::IRCMessage(std::string &message, SocketClient *socket) : _is_valid(Category::UNKNOWN), _socket(socket)
{
    this->splitIRCMessage(message);
}

IRCMessage::IRCMessage(IRCMessage const &instance)
{
    *this = instance;
}

IRCMessage &IRCMessage::operator=(IRCMessage const &rhs)
{
    this->_prefix = rhs.getPrefix();
    this->_command = rhs.getCommand();
    this->_parameters = rhs.getParameters();
    this->_trail = rhs.getTrail();
    this->params = rhs.params;
    return *this;
}

IRCMessage::~IRCMessage(void)
{
    return;
}

IRCMessage &IRCMessage::setPrefix(std::string const &prefix, IRCMessageWay w)
{
    this->_prefix = prefix;
    if (w == IRCMessageWay::RECIEVING && !prefix.empty())
    {
        this->_prefix.erase(0, 1);
        std::size_t idxSpace = this->_prefix.rfind(" ");
        (idxSpace != std::string::npos) ? this->_prefix.erase(idxSpace, 1) : 0;
    }
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
        this->_is_valid = Category::COMMAND;
    }
    else
    {
        std::regex e("\\d{3}");
        if (std::regex_match(command, e))
            this->_is_valid = Category::NUMERIC_REPLY;
        else
            this->_is_valid = Category::UNKNOWN;
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

IRCMessage &IRCMessage::setParameters(std::vector<std::string> const &params)
{
    this->_parameters = params;
    return (*this);
}

IRCMessage &IRCMessage::setTrail(std::string const &trail, IRCMessageWay w)
{
    this->_trail = trail;
    (w == IRCMessageWay::RECIEVING) ? this->_trail.erase(0, 1) : this->_trail;
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

bool IRCMessage::isCommand(SocketClient *socket)
{
    if (this->_is_valid == Category::COMMAND)
    {
        if (this->type == IRCMessageType::SERVICE && _parameters.size() >= 1)
            params.nickname = _parameters[0];
        else if (this->type == IRCMessageType::USER && _parameters.size() >= 3)
        {
            params.user = _parameters[0];
            params.real_name = _trail;
            params.host = IRCServer::name;
            // Attention crash si non digit
            try
            {
                params.modeint = std::stoi(_parameters[1]);
            }
            catch (const std::invalid_argument &e)
            {
                IRCServer::_reply_manager->reply(Parameters(*this), ReplyManager::ERR_MALFORMEDPARAMS, socket);
                return (false);
            }
        }
        else if (this->type == IRCMessageType::NICK && _parameters.size() >= 1)
        {
            params.prevNickname = _prefix;
            params.nickname = _parameters[0];
            if (_parameters.size() >= 6)
            {
                try
                {
                    params.hopcount = std::stoi(_parameters[1]);
                    params.user = _parameters[2];
                    params.host = _parameters[3];
                    params.token = std::stoi(_parameters[4]);
                    params.modestr = _parameters[5];
                    params.real_name = _trail;
                }
                catch (const std::invalid_argument &e)
                {
                    IRCServer::_reply_manager->reply(Parameters(*this), ReplyManager::ERR_MALFORMEDPARAMS, socket);
                    return (false);
                }
            }
        }
        else if (this->type == IRCMessageType::PASS && _parameters.size() >= 1)
            params.password = _parameters[0];
        else if (this->type == IRCMessageType::OPER && _parameters.size() == 2)
        {
            params.user = _parameters[0];
            params.password = _parameters[1];
        }
        else if (this->type == IRCMessageType::JOIN && _parameters.size() >= 1)
        {
            params.channelName = _parameters[0];
            params.keys = (_parameters.size() >= 2 ? _parameters[0] : params.keys);
        }
        else if (this->type == IRCMessageType::LUSERS)
        {
            if (!_parameters.empty())
                params.target = _parameters[0];
        }
        else if (this->type == IRCMessageType::PART && _parameters.size() >= 1)
        {
            params.channelName = _parameters[0];
            params.leave_message = (_parameters.size() >= 2 ? _parameters[0] : params.leave_message);
        }
        else if (this->type == IRCMessageType::QUIT)
        {
            if (_parameters.size() == 1)
                params.quit_message = _parameters[0];
        }
        else if (this->type == IRCMessageType::SERVER)
        {
            if (_parameters.size() >= 3){
                    params.newServer = _parameters[0];
                try
                {
                    params.hopcount = std::stoi(_parameters[1]);
                    params.token = std::stoi(_parameters[2]);
                }
                catch (std::invalid_argument &e)
                {
                    IRCServer::_reply_manager->reply(Parameters(*this), ReplyManager::ERR_MALFORMEDPARAMS, socket);
                    std::cout << e.what() << std::endl;
                    return (false);
                }
                params.serverInfo = this->_trail;
                params.uplink = this->_prefix.empty() ? IRCServer::name : this->prefix ;
            }
        }
        else if (this->type == IRCMessageType::PRIVMSG)
        {
            if (_parameters.size() >= 1)
            {
                params.target = _parameters[0];
                if (!_trail.empty())
                {
                    params.text = _trail;
                }
                else
                {
                    IRCServer::_reply_manager->reply(Parameters(*this), ReplyManager::ERR_NOTEXTTOSEND, socket);
                    return (false);
                }
            }
            else
            {
                IRCServer::_reply_manager->reply(Parameters(*this), ReplyManager::ERR_NORECIPIENT, socket);
                return (false);
            }
        }
        else
        {
            IRCServer::_reply_manager->reply(Parameters(*this), ReplyManager::ERR_NEEDMOREPARAMS, socket);
            return (false);
        }
        return (true);
    }
    else if (this->_is_valid == Category::UNKNOWN)
        IRCServer::_reply_manager->reply(Parameters(*this), ReplyManager::ERR_UNKNOWNCOMMAND, socket);
    return (false);
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
        setPrefix(cm[1], IRCMessageWay::RECIEVING).setCommand(cm[2]).setParameters(cm[3]).setTrail(cm[4], IRCMessageWay::RECIEVING);
}

std::string IRCMessage::getCommand() const
{
    return this->_command;
}

std::string IRCMessage::getPrefix() const
{
    return this->_prefix;
}

std::string IRCMessage::getTrail() const
{
    return this->_trail;
}

std::vector<std::string> IRCMessage::getParameters() const
{
    return (this->_parameters);
};

void IRCMessage::setSocket(SocketClient *socket)
{
    this->_socket = socket;
}

SocketClient *IRCMessage::getSocket(void) const
{
    return this->_socket;
}

std::string IRCMessage::to_string(void) const
{
    std::string result;
    !(this->_prefix.empty()) ? result += ":" + this->_prefix : result;
    this->_command.empty() ? throw std::logic_error("Error : no command in automatic IRCMessage") : 0;
    !result.empty() ? result += ' ' : result;
    result += this->_command;
    this->_parameters.empty() ? throw std::logic_error("Error : no parameters in automatic IRCMessage") : 0;
    std::for_each(this->_parameters.begin(), this->_parameters.end(), [&result](std::string const &str) { result += ' ' + str; });
    !(this->_trail.empty()) ? result += " :" + this->_trail : result;
    return result + '\n';
}

std::ostream &operator<<(std::ostream &os, const IRCMessage &message)
{
    os << message.getMessage();
    return os;
}