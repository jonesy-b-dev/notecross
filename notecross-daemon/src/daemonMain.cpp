#include "daemonizer.hpp"
#include "notificationCalculator.hpp"
#include "task.hpp"
#include <chrono>
#include <log.hpp>
#include <string>
// #include <glib-2.0/glib.h">
#include "filewatcher.hpp"
#include <libnotify/notify.h>
#include <unistd.h>

void CheckTaskToNotify(NCShared::Task taskToNotify);

int main()
{
    NCDaemon::KillOtherNCDaemonProcesses();

    pid_t notecrossDaemon = NCDaemon::Daemonize();

    if (notecrossDaemon == -1)
    {
        NCShared::LogFileError("Failed to daemonize process with pid: " + std::to_string(getpid()));
        exit(1);
    }

    std::chrono::duration duration = std::chrono::system_clock::now().time_since_epoch();
    long currentUnixTime = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    // Initial startup check
    NCShared::Task taskToNotify = CalculateNextNotification(currentUnixTime);
    CheckTaskToNotify(taskToNotify);

    std::string directoryPath = std::string(std::getenv("HOME")) + "/.notecross/";
    std::string targetFileName = "tasks.json";
    int inotifyHandle = initializeFileWatcher(directoryPath + targetFileName);

    const size_t eventSize = sizeof(struct inotify_event);
    const size_t bufferLength = 4 * (eventSize + 16);
    char eventBuffer[bufferLength];

    while (true)
    {
        ssize_t bytesRead = read(inotifyHandle, eventBuffer, bufferLength);
        if (bytesRead < 0)
        {
            NCShared::LogFileError("Read error");
            break;
        }

        size_t bufferOffset = 0;
        while (bufferOffset < static_cast<size_t>(bytesRead))
        {
            struct inotify_event* eventPointer =
                reinterpret_cast<struct inotify_event*>(&eventBuffer[bufferOffset]);

            if (eventPointer->len > 0 && targetFileName == eventPointer->name)
            {
                if (eventPointer->mask & IN_MODIFY)
                {
                    std::chrono::duration duration =
                        std::chrono::system_clock::now().time_since_epoch();
                    long currentUnixTime =
                        std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                    NCShared::LogFileMessage("Task File updated, rechecking tasks...");
                    taskToNotify = CalculateNextNotification(currentUnixTime);
                    CheckTaskToNotify(taskToNotify);
                }
            }

            bufferOffset += eventSize + eventPointer->len;
        }
    }
}

void CheckTaskToNotify(NCShared::Task taskToNotify)
{
    NCShared::LogFileMessage(std::to_string(taskToNotify.dueDate));
    if (taskToNotify.dueDate == 9999999999)
    {
        return;
    }
    else
    {
        std::chrono::duration duration = std::chrono::system_clock::now().time_since_epoch();
        long currentUnixTime = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        NCShared::LogFileMessage("Next task to notify: Description: " + taskToNotify.description);
        NCShared::LogFileMessage("Next notification if not interupted in: " +
                                 std::to_string(taskToNotify.dueDate - currentUnixTime));

        sleep(taskToNotify.dueDate - currentUnixTime + 1);
        notify_init("Task is due!");
        NotifyNotification* n = notify_notification_new(taskToNotify.description.c_str(), " ", 0);
        notify_notification_set_timeout(n, 5000); // 5 seconds

        if (!notify_notification_show(n, 0))
        {
            NCShared::LogFileError("Failed to show notification");
        }

        taskToNotify = CalculateNextNotification(currentUnixTime);
    }
}
