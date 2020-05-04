#ifndef ICLIENTSTATE_CLASS_HPP
# define ICLIENTSTATE_CLASS_HPP
#include "IRCMessage.class.hpp"
class IClientState
{
	public:
		virtual void handle(IRCMessage const &message) = 0;
};

#endif