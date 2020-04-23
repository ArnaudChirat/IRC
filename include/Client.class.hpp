#ifndef CLIENT_CLASS_HPP
# define CLIENT_CLASS_HPP
#include <string>
class Client
{
	public:
		Client(void);
		// Client(** replace parameters **);
		Client(Client const &instance);
		Client &operator=(Client const &rhs);
		~Client(void);
        void recvMessage(std::string message);

	private:
        std::string _name;
        unsigned int _privilege;
};

enum ClientPrivilige
{
    CREATOR = 1,
    OPERATOR = 1 << 1,
    VOICE = 1 << 2,
};

#endif