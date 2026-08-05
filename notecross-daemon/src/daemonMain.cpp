#include "daemonizer.hpp"
#include <iostream>
#include <unistd.h>

int main()
{
    NCDaemon::KillOtherNCDaemonProcesses();

    pid_t notecrossDaemon = NCDaemon::Daemonize();

    if (notecrossDaemon == -1)
    {
        std::cerr << "Failed to daemonize process with pid: " << getpid() << std::endl;
        exit(1);
    }
}
