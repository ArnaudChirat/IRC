#include "RoutingTable.class.hpp"
#include <iostream>
RoutingTable::RoutingTable()
{
}

RoutingTable::~RoutingTable()
{
}

void RoutingTable::addRoute(ServerClient const &server, ServerClient const &newServer)
{
    std::pair<Token, Token> value(newServer.getToken(), server.getToken());
    _routing_table.insert(value);
}

Token RoutingTable::getRoute(Token token) const
{
   return _routing_table.at(token);
}

void RoutingTable::delRoute(Token token)
{
    for (auto i = _routing_table.begin(); i != _routing_table.end(); i++)
    {
        if (i->second == token)
            _routing_table.erase(i->first);
    }
    _routing_table.erase(token);
}

std::vector<std::pair<Token, Token>> RoutingTable::getRoutingTable() const
{
    return std::vector<std::pair<Token, Token>>(_routing_table.begin(), _routing_table.end());
};

std::ostream &operator<<(std::ostream &o, RoutingTable const &rhs)
{
    auto table = rhs.getRoutingTable();
    std::for_each(table.begin(), table.end(), [&o](std::pair<Token,Token> const &v){
        o << "To talk to server " << v.first << " pass by " << v.second << std::endl; 
    });
    return (o);
}