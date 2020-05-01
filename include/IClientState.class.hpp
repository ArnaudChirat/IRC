#ifndef ICLIENTSTATE_CLASS_HPP
# define ICLIENTSTATE_CLASS_HPP

class IClientState
{
	public:
		virtual void handle() = 0;
};

#endif