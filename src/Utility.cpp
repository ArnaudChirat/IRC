#include "Utility.hpp"
#include <errno.h>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
int Utility::safe_dup(int fd) {
    int copy = dup(fd); 
    if (copy < 0)
        throw std::runtime_error(strerror(errno));
    return copy;
}