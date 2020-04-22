#ifndef CLIENT_CLASS_HPP
# define CLIENT_CLASS_HPP

class Client
{
	public:
		Client(void);
		// Client(** replace parameters **);
		Client(Client const &instance);
		Client &operator=(Client const &rhs);
		~Client(void);

	private:

};

#endif