#ifndef CLIENT_CLASS_HPP
#define CLIENT_CLASS_HPP
#include <string>
#include "SocketClient.class.hpp"
class Client
{
public:
    Client(SocketClient *socket_client);
    virtual ~Client(void);
    virtual Client &setName(std::string const &name) = 0;
    std::string getName() const;
    enum class Status
    {
        CONNECTING,
        CONNECTED,
        DISCONNECTED
    };
protected:
    SocketClient *_socket_client;
    Status status;
    std::string _name;
private:
    Client(void);
};

#endif