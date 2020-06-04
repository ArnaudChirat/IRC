#ifndef ROUTINGTABLE_CLASS_HPP
#define ROUTINGTABLE_CLASS_HPP
#include "Utility.hpp"
#include <unordered_map>
#include "ServerClient.class.hpp"
#include <vector>
class RoutingTable
{
public:
    RoutingTable(void);
    // RoutingTable(**replace parameters **);
    // RoutingTable(RoutingTable const &instance);
    // RoutingTable &operator=(RoutingTable const &rhs);
    ~RoutingTable(void);
    void addRoute(ServerClient const &, ServerClient const &);
    Token getRoute(Token) const;
    void delRoute(Token);
    std::vector<std::pair<Token, Token>> getRoutingTable() const;

private:
    std::unordered_map<Token, Token> _routing_table;
};

std::ostream &operator<<(std::ostream &o, RoutingTable const &rhs);

#endif