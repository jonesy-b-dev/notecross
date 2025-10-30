#include "Task.h"
#include "json.hpp"
#include "log.hpp"
#include "taskHelper.hpp"
#include <algorithm>
#include <fstream>
// #include <glib-2.0/glib.h">
#include <iterator>
#include <libnotify/notify.h>

#define TASKDIR "~/.notecross/"

using json = nlohmann::json;

namespace Daemon
{
std::string TaskGetAll()
{
    std::ifstream tasksFile = OpenTaskFileRead();
    if (!tasksFile.is_open())
        return "Failed to openfile, check /tmp/notecross.log for more details";

    // Read entire file into a string
    std::string result =
        std::string(std::istreambuf_iterator<char>(tasksFile), std::istreambuf_iterator<char>());
    return result;
}

std::string TaskGetAllFormatted()
{
    std::ifstream tasksFile = OpenTaskFileRead();
    if (!tasksFile.is_open())
        return "Failed to openfile, check /tmp/notecross.log for more details";

    json taskData = json::parse(tasksFile);
    tasksFile.close();

    if (!taskData.contains("tasks") || !taskData["tasks"].is_array())
        return "No tasks found.";

    std::ostringstream output;

    int count = 0;
    for (const auto& task : taskData["tasks"])
    {
        ++count;
        int id = task.value("id", 0);
        std::string desc = task.value("task", "<no description>");
        output << std::setw(2) << id << ". " << desc << "\n";
    }

    if (count == 0)
        return "No tasks available.";

    return output.str();
}

std::string TaskAdd(std::string newTask, std::string taskDue)
{
    std::ifstream tasksFile = OpenTaskFileRead();
    if (!tasksFile.is_open())
        return "Failed to open file for read, check /tmp/notecross.log for more details";

    json taskData = json::parse(tasksFile);
    tasksFile.close();

    int nextId = taskData["tasks"].back()["id"];
    nextId++;
    Daemon::LogMessage("Next id is:" + std::to_string(nextId));

    int unixDueDate = TaskDueToUnixTime(taskDue);
    if (unixDueDate == -1)
    {
        return "Failed to parse due date, check GitHub for correct format";
    }
    json newTaskJson = {{"id", nextId}, {"task", newTask}, {"due date", taskDue}};

    taskData["tasks"].push_back(newTaskJson);

    std::ofstream tasksFileWrite = OpenTaskFileWrite();
    if (!tasksFileWrite.is_open())
        return "Failed to openfile, check /tmp/notecross.log for more details";
    tasksFileWrite << taskData.dump(4);

    tasksFile.close();

    Daemon::LogMessage("Added new task: " + newTask);

    // NOTIFICATION
    notify_init("Task Added");
    NotifyNotification* n = notify_notification_new(newTask.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000); // 5 seconds

    if (!notify_notification_show(n, 0))
    {
        Daemon::LogError("Failed to show notification");
        return "Added new task but failed to show notification";
    }

    return "Added new task.";
}

std::string TaskUpdate(int id, Task updatedTask);
std::string TaskRemove(int id)
{
    std::ifstream tasksFile = OpenTaskFileRead();
    if (!tasksFile.is_open())
        return "Failed to open file for read, check /tmp/notecross.log for more details";

    json taskData = json::parse(tasksFile);
    tasksFile.close();

    json& tasks = taskData["tasks"];
    auto newEnd =
        std::remove_if(tasks.begin(),
                       tasks.end(),
                       [id](const json& task) { return task.contains("id") && task["id"] == id; });

    tasks.erase(newEnd, tasks.end());

    std::ofstream tasksFileWrite = OpenTaskFileWrite();
    if (!tasksFileWrite.is_open())
        return "Failed to openfile, check /tmp/notecross.log for more details";
    tasksFileWrite << taskData.dump(4);

    tasksFile.close();

    Daemon::LogMessage("Removed task with id: " + std::to_string(id));

    // NOTIFICATION
    notify_init("Task Removed");
    std::string message = "Removed task with id: " + std::to_string(id);
    NotifyNotification* n = notify_notification_new(message.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000);

    if (!notify_notification_show(n, 0))
    {
        Daemon::LogError("Failed to show notification");
        return "Added new task but failed to show notification";
    }
    return "Removed task with id: " + std::to_string(id);
}
std::string TaskSync();
} // namespace Daemon
