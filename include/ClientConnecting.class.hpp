#ifndef CLIENTCONNECTING_CLASS_HPP
# define CLIENTCONNECTING_CLASS_HPP
#include "IClientState.class.hpp"
#include "Client.class.hpp"
class ClientConnecting : public IClientState
{
	public:
		ClientConnecting(Client *context);
		// ClientConnecting(ClientConnecting const &instance);
		// ClientConnecting &operator=(ClientConnecting const &rhs);
		~ClientConnecting(void);
        void handle(IRCMessage const &message);
	private:
		ClientConnecting(void);
        Client *_context;
};

#endif