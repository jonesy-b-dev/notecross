#include "daemonizer.hpp"
#include <iostream>
#include <unistd.h>

int main()
{
    std::cout << "Started daemon!\n";
    pid_t notecrossDaemon = Daemonize();

    if (notecrossDaemon == -1)
    {
        std::cerr << "Failed to daemonize process with pid: " << getpid() << std::endl;
    }
}
