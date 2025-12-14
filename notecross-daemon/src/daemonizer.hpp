#pragma once
#include <sched.h>

namespace NCDaemon
{
void KillOtherNCDaemonProcesses();
pid_t Daemonize();
}
