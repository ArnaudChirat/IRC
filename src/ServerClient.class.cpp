#include "ServerClient.class.hpp"
#include "Utility.hpp"

ServerClient::ServerClient(SocketClient * socket): Client(socket) {}

ServerClient::~ServerClient(void) {}

ServerClient & ServerClient::setName(std::string const &name){
    this->_name = name;
    return *this;
}

void    ServerClient::setServerInfo(Parameters const & params) {
    this->_uplink = params.host;
    this->_hopcount = params.hopcount;
    this->_token = params.token;
    this->_info = params.serverInfo;
}

std::string         ServerClient::getUplink(void) const{
    return this->_uplink;
}
unsigned int    ServerClient::getHopcount(void) const{
    return this->_hopcount;
    
}
unsigned int    ServerClient::getToken(void) const{
    return this->_token;
    
}
std::string     ServerClient::getInfo(void) const{
    return this->_info;
    
}

void ServerClient::addServer(Token token, ServerClient &server)
{
    std::pair<Token, ServerClient *> value(token, &server);
    this->_servers.insert(value);
}

std::map<Token, ServerClient*> ServerClient::getServers() const
{
    return (this->_servers);
}

std::ostream &   operator<<(std::ostream & o, ServerClient const & rhs){
    o << "server name : " << rhs.getName() << std::endl;
    o << "password : " << rhs.getPassword() << std::endl;
    o << "uplink : " << rhs.getUplink() << std::endl;
    o << "hopcount : " << rhs.getHopcount() << std::endl;
    o << "token : " << rhs.getToken() << std::endl;
    o << "info : " << rhs.getInfo() << std::endl;
    auto servers = rhs.getServers();
    o << "other servers : " << std::endl;
    for (auto i = servers.begin(); i != servers.end(); i++)
    {
        o << i->first << " : " << i->second->getName() << std::endl;
    }
    return o;
}
