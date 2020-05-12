#ifndef SERVICE_CLASS_HPP
# define SERVICE_CLASS_HPP
#include "Client.class.hpp"
class Service : public Client
{
	public:
        Service(SocketClient *socket_client);
		Service(Service const &instance);
		Service &operator=(Service const &rhs);
		~Service(void);
        Service & setName(std::string const &name);        

	private:
		Service(void);
};

#endif