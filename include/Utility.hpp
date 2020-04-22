#ifndef UTILITY_H
#define UTILITY_H
#include <netinet/in.h>
struct Socket
{
	int sckt;
	sockaddr_in addr;
};

namespace Utility 
{
    int safe_dup(int fd);
}
#endif
