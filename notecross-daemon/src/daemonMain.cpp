#include "daemonizer.hpp"
#include "notificationCalculator.hpp"
#include "task.hpp"
#include <chrono>
#include <log.hpp>
#include <string>
// #include <glib-2.0/glib.h">
#include <libnotify/notify.h>
#include <unistd.h>

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

    while (1)
    {
        if (taskToNotify.dueDate == 9999999999)
        {
            // Wait for task file update
        }
        else
        {
            NCShared::LogFileMessage("Next task to notify: Description: " +
                                     taskToNotify.description);
            NCShared::LogFileMessage("Next notification if not interupted in: " +
                                     std::to_string(taskToNotify.dueDate - currentUnixTime));

            sleep(taskToNotify.dueDate - currentUnixTime + 1);
            notify_init("Task is due!");
            NotifyNotification* n =
                notify_notification_new(taskToNotify.description.c_str(), " ", 0);
            notify_notification_set_timeout(n, 5000); // 5 seconds

            if (!notify_notification_show(n, 0))
            {
                NCShared::LogFileError("Failed to show notification");
            }
            std::chrono::time_point now = std::chrono::system_clock::now();
            std::chrono::duration duration = now.time_since_epoch();
            long currentUnixTime =
                std::chrono::duration_cast<std::chrono::seconds>(duration).count();

            NCShared::Task taskToNotify = CalculateNextNotification(currentUnixTime);
        }
    }

    // Calculate earliest noti that needs to be shown
    // Calculate when:
    // Task file gets updated
    // Deamon starts
    // Notification has been send
    // Wait till noti needs to be send
    // Send noti
    // Repeat

    // CalcFunc
    // Check all due dates
}
