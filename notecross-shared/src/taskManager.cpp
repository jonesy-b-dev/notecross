#include "Task.h"
#include "include/taskHelper.hpp"
#include "json.hpp"
#include "log.hpp"
#include <algorithm>
#include <fstream>
// #include <glib-2.0/glib.h">
#include <iterator>
#include <libnotify/notify.h>

#define TASKDIR "~/.notecross/"

using json = nlohmann::json;

namespace NCShared
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

    if (!taskData.contains("tasks") || !taskData["tasks"].is_array() || taskData["tasks"].empty())
        return "No tasks found.";

    // Find longest description
    size_t maxDesc = 0;
    for (const auto& task : taskData["tasks"])
    {
        std::string desc = task.value("task", "<no description>");
        maxDesc = std::max(maxDesc, desc.size());
    }

    std::ostringstream output;
    for (const auto& task : taskData["tasks"])
    {
        int id = task.value("id", 0);
        std::string desc = task.value("task", "<no description>");
        std::string due = "";

        const auto& dueField = task["due date"];

        if (dueField.is_number())
        {
            due = NCShared::TaskDueToDate(dueField.get<int>());
        }
        else if (dueField.is_string())
        {
            due = dueField.get<std::string>(); // or parse/convert it
        }

        output << "  " << id << ". " << std::left << std::setw(maxDesc) << desc << " | Due: " << due
               << "\n";
    }

    NCShared::LogMessage("Listed all tasks.");

    return output.str();
}

std::string TaskAdd(std::string newTask, std::string taskDue)
{
    NCShared::LogMessage("Adding new task....");

    std::ifstream tasksFile = OpenTaskFileRead();
    if (!tasksFile.is_open())
        return "Failed to open file for read, check /tmp/notecross.log for more details";

    json taskData = json::parse(tasksFile);
    tasksFile.close();
    NCShared::LogMessage("Parsed and closed tasksFile");

    int nextId = 0;
    nextId =
        !taskData.contains("tasks") || !taskData["tasks"].is_array() || taskData["tasks"].empty()
            ? 1
            : taskData["tasks"].back().value("id", 0) + 1;
    NCShared::LogMessage("Next id is:" + std::to_string(nextId));

    json newTaskJson;

    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::duration duration = now.time_since_epoch();
    auto currentUnixTime = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    if (taskDue.empty())
    {
        newTaskJson = {{"id", nextId},
                       {"task", newTask},
                       {"due date", "No due set"},
                       {"creation date", currentUnixTime}};
    }
    else
    {
        int unixDueDate = TaskDueToUnixTime(taskDue);

        if (unixDueDate == -1)
        {
            return "Failed to parse due date, check `/tmp/notecross.log for more info and check "
                   "GitHub for correct format";
        }

        newTaskJson = {{"id", nextId},
                       {"task", newTask},
                       {"due date", unixDueDate},
                       {"creation date", currentUnixTime}};
    }

    taskData["tasks"].push_back(newTaskJson);

    std::ofstream tasksFileWrite = OpenTaskFileWrite();
    if (!tasksFileWrite.is_open())
        return "Failed to openfile, check /tmp/notecross.log for more details";
    tasksFileWrite << taskData.dump(4);

    tasksFile.close();

    NCShared::LogMessage("Added new task: " + newTask);

    // NOTIFICATION
    notify_init("Task Added");
    NotifyNotification* n = notify_notification_new(newTask.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000); // 5 seconds

    if (!notify_notification_show(n, 0))
    {
        NCShared::LogError("Failed to show notification");
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

    NCShared::LogMessage("Removed task with id: " + std::to_string(id));

    // NOTIFICATION
    notify_init("Task Removed");
    std::string message = "Removed task with id: " + std::to_string(id);
    NotifyNotification* n = notify_notification_new(message.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000);

    if (!notify_notification_show(n, 0))
    {
        NCShared::LogError("Failed to show notification");
        return "Added new task but failed to show notification";
    }
    return "Removed task with id: " + std::to_string(id);
}
std::string TaskSync();
} // namespace NCShared
