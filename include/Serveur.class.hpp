#ifndef SERVEUR_HPP
#define SERVEUR_HPP
#include <sys/socket.h>
#include <string>
#include <forward_list>
#include <netinet/in.h>

struct Socket
{
	int sckt;
	sockaddr_in addr;
};

class Serveur
{
public:
	Serveur(void);
	Serveur(Serveur const &instance);
	Serveur &operator=(Serveur const &rhs);
	virtual ~Serveur(void);
	void launchServeur(unsigned short port);

private:
	std::forward_list<Socket> _sockets;
	int _serveur_socket;
	fd_set _readfds, _writefds, _errorfds;
	int _max_fd;
	void setFdSet();
	std::string getAddress(const sockaddr_in &addr) const;
	void acceptNewClient();
	void createServeur(unsigned short port);
	bool clientSocket(const std::string &socket_address, const int socket_port, std::forward_list<Socket>::iterator it) const;
};

#endif