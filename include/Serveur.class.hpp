#ifndef SERVEUR_HPP
#define SERVEUR_HPP
#include <sys/socket.h>
#include <string>
#include <list>
#include <netinet/in.h>
#include "Utility.hpp"
#include "Socket.class.hpp"
#include "Client.class.hpp"
class Serveur : public Socket
{
public:
	Serveur(unsigned short port);
	Serveur(Serveur const &instance) = delete;
	Serveur &operator=(Serveur const &rhs) = delete;
	~Serveur(void);
	Client *acceptNewClient();
	void handle(SocketManagerInterface &dispatcher);
private:
	Serveur(void);
	// void recvMessage() const;
	// void dispatchMessageToChannel();
	// void sendMessageToClient();
};
#endif