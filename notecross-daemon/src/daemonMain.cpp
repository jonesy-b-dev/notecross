#include <iostream>
#include <sched.h>
#include <unistd.h>

int main()
{
    std::cout << "Started daemon!\n";

    pid_t childPid = fork();

    if (childPid == -1)
    {
        std::cerr << "First fork failed!" << std::endl;
        return 1;
    }
    if (childPid > 0)
        return 1;
    // Child process

    std::cout << "Child process, PID: " << getpid() << std::endl;

    setsid();

    pid_t notecrossDaemon = fork();
    if (notecrossDaemon == -1)
    {
        std::cerr << "First fork failed!" << std::endl;
        return 1;
    }
    if (notecrossDaemon > 0)
        return 1;

    std::cout << "Notecross Daemon process, PID: " << getpid() << std::endl;
}
