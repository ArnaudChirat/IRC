#ifndef CLIENT_CLASS_HPP
#define CLIENT_CLASS_HPP
#include <string>
#include "SocketClient.class.hpp"
class Client
{
public:
    Client(SocketClient *socket_client);
    Client &operator=(Client const &rhs);
    virtual ~Client(void);
    virtual Client & setName(std::string const &name) = 0;
    SocketClient * getSocketClient(void) const;
    std::string getName() const;
    std::string getPrevName() const;
    void setPassword(std::string const &pass);
    std::string getPassword() const;
    SocketClient *_socket_client;
    enum class Status
    {
        CONNECTING,
        CONNECTED,
        DISCONNECTED
    };
    Status status;
protected:
    std::string _prevName;
    std::string _name;
    Client(void);

private:
    std::string _password;

};

#endif