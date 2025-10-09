#pragma once
#include <iostream>
#include <sched.h>
#include <unistd.h>

pid_t Daemonize()
{
    // First Child process
    pid_t childPid = fork();

    if (childPid == -1)
    {
        std::cerr << "First fork failed!" << std::endl;
        return -1;
    }
    if (childPid > 0)
        return -1;

    std::cout << "First child process, PID: " << getpid() << std::endl;

    setsid();

    // Acutual daemon process
    pid_t secondChild = fork();
    if (secondChild == -1)
    {
        std::cerr << "Second fork failed!" << std::endl;
        return -1;
    }
    if (secondChild > 0)
        return -1;

    std::cout << "Notecross Daemon process, PID: " << getpid() << std::endl;
    return secondChild;
}
