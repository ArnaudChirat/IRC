#ifndef MESSAGEMEDIATOR_CLASS_HPP
# define MESSAGEMEDIATOR_CLASS_HPP
#include "IRCMessage.class.hpp"
class MessageMediator
{
	public:
		MessageMediator(void);
		// MessageMediator(MessageMediator const &instance);
		// MessageMediator &operator=(MessageMediator const &rhs);
        ~MessageMediator(void);
		bool handleMessage(IRCMessage const &message) const;
        void createClient(IRCMessage const &message) const;
	private:
};

#endif