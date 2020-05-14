#ifndef CLIENT_CLASS_HPP
#define CLIENT_CLASS_HPP
#include <string>
#include "SocketClient.class.hpp"
class Client
{
public:
    Client(SocketClient *socket_client);
    virtual ~Client(void);
    virtual Client & setName(std::string const &name) = 0;
    SocketClient * getSocketClient(void) const;
    virtual std::string getName() const;
    SocketClient *_socket_client;
    enum class Status
    {
        CONNECTING,
        CONNECTED,
        DISCONNECTED
    };
    Status status;
protected:
    std::string _name;
private:
    Client(void);
};

#endif