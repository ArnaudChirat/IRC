#ifndef UTILITY_H
#define UTILITY_H
#include <netinet/in.h>

#define BUFF_MSG 512
namespace Utility 
{
    int safe_dup(int fd);
}
#endif
