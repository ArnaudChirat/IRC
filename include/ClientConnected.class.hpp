#ifndef CLIENTCONNECTED_CLASS_HPP
# define CLIENTCONNECTED_CLASS_HPP
#include "IClientState.class.hpp"
#include "Client.class.hpp"
class ClientConnected : public IClientState
{
	public:
		ClientConnected(Client *context);
		// ClientConnected(ClientConnected const &instance);
		// ClientConnected &operator=(ClientConnected const &rhs);
		~ClientConnected(void);
        void handle(IRCMessage const &message);
	private:
		ClientConnected(void);
        Client *_context;
};

#endif