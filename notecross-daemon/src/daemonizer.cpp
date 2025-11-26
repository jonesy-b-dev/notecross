#include "log.hpp"
#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <sched.h>
#include <unistd.h>

namespace NCDaemon
{
void SignalHandler(int sig)
{
    switch (sig)
    {
    case SIGHUP:
        std::cout << "Hangup signal caught\n";
        break;
    case SIGTERM:
        std::cout << "Terminate signal caught\n";
        exit(0);
        break;
    }
}
pid_t Daemonize()
{
	std::cout << "Starting Daemonization\n";
    // First Child process
    pid_t childPid = fork();

    if (childPid == -1)
    {
        std::cerr << "First fork failed!" << std::endl;
        exit(1);
    }
    if (childPid > 0)
        exit(1);

    std::cout << "First child process, PID: " << getpid() << std::endl;

    setsid();

    // Acutual daemon process
    pid_t secondChild = fork();
    if (secondChild == -1)
    {
        std::cerr << "Second fork failed!" << std::endl;
        exit(1);
    }
    if (secondChild > 0)
        exit(1);

    std::cout << "Daemon process, PID: " << getpid() << std::endl;

    chdir("/");

    // Close file discriptors
    for (int i = 0; i < getdtablesize(); i++)
    {
        close(i);
    }

    // Reopen file discriptors
    // stdin
    open("/dev/null", O_RDONLY);

    // stdout
    int fd = open("/var/notecross/notecross.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        open("/tmp/notecross.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    }

    // stderr
    dup2(1, 2);

    std::cout.clear();
    std::cerr.clear();

    signal(SIGCHLD, SIG_IGN); /* Ignore child */
    signal(SIGTSTP, SIG_IGN); /* Ignore tty signals */
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, SignalHandler);  /* Catch hangup signal */
    signal(SIGTERM, SignalHandler); /* Catch kill signal */

    std::cout << "\n\n";
    NCDaemon::LogMessage("Daemonization complete");

    return secondChild;
}
} // namespace NCDaemon
