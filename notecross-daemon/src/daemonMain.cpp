#include "daemonizer.hpp"
#include "notificationCalculator.hpp"
#include "task.hpp"
#include <chrono>
#include <log.hpp>
#include <string>
// #include <glib-2.0/glib.h">
#include <unistd.h>
#include <libnotify/notify.h>

int main()
{
    NCDaemon::KillOtherNCDaemonProcesses();

    pid_t notecrossDaemon = NCDaemon::Daemonize();

    if (notecrossDaemon == -1)
    {
        NCShared::LogFileError("Failed to daemonize process with pid: " + std::to_string(getpid()));
        exit(1);
    }
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::duration duration = now.time_since_epoch();
    long currentUnixTime = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

	NCShared::Task taskToNotify = CalculateNextNotification(currentUnixTime);

    if (taskToNotify.dueDate != 9999999999)
    {
		NCShared::LogFileMessage("Next task to notify: Description: " + taskToNotify.description);
        notify_init("Task is due!");
        NotifyNotification* n = notify_notification_new(taskToNotify.description.c_str(), " ", 0);
        notify_notification_set_timeout(n, 5000); // 5 seconds

        if (!notify_notification_show(n, 0))
        {
            NCShared::LogFileError("Failed to show notification");
        }
    }

}
