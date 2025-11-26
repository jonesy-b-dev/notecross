#include "daemonizer.hpp"
#include "socketHandler.hpp"
#include <iostream>
#include <unistd.h>

int main()
{
    pid_t notecrossDaemon = NCDaemon::Daemonize();

    if (notecrossDaemon == -1)
    {
        std::cerr << "Failed to daemonize process with pid: " << getpid() << std::endl;
        exit(1);
    }
    int socketFileDiscriptor = NCDaemon::OpenSocket();
    NCDaemon::HandleConnections(socketFileDiscriptor);
}
