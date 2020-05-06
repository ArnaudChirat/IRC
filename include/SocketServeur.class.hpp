#ifndef SOCKET_SERVEUR_HPP
#define SOCKET_SERVEUR_HPP
#include <sys/socket.h>
#include <string>
#include <list>
#include <netinet/in.h>
#include "Utility.hpp"
#include "Socket.class.hpp"
#include "SocketClient.class.hpp"
class SocketServeur : public Socket
{
public:
	SocketServeur(unsigned short port);
	SocketServeur(SocketServeur const &instance) = delete;
	SocketServeur &operator=(SocketServeur const &rhs) = delete;
	~SocketServeur(void);
	SocketClient *acceptNewClient();
	void handle(SocketManagerInterface &dispatcher);
private:
	SocketServeur(void);
	// void recvMessage() const;
	// void dispatchMessageToChannel();
	// void sendMessageToClient();
};
#endif