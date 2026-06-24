#include "include/log.hpp"
#include "include/taskHelper.hpp"
#include "json.hpp"
#include <algorithm>
#include <fstream>
// #include <glib-2.0/glib.h">
#include <libnotify/notify.h>

#define TASKDIR "~/.notecross/"

using json = nlohmann::json;

namespace NCShared
{

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

    output << "== Current tasks ==\n";
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

    NCShared::LogFileMessage("Listed all tasks.");

    return output.str();
}

std::string TaskAdd(std::string newTask, std::string taskDue)
{
    NCShared::LogFileMessage("Adding new task....");

    std::ifstream tasksFile = OpenTaskFileRead();
    if (!tasksFile.is_open())
        return "Failed to open file for read, check /tmp/notecross.log for more details";

    json taskData = json::parse(tasksFile);
    tasksFile.close();
    NCShared::LogFileMessage("Parsed and closed tasksFile");

    int nextId = 0;
    nextId =
        !taskData.contains("tasks") || !taskData["tasks"].is_array() || taskData["tasks"].empty()
            ? 1
            : taskData["tasks"].back().value("id", 0) + 1;
    NCShared::LogFileMessage("Next id is:" + std::to_string(nextId));

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

    NCShared::LogFileMessage("Added new task: " + newTask);

    // NOTIFICATION
    notify_init("Task Added");
    NotifyNotification* n = notify_notification_new(newTask.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000); // 5 seconds

    if (!notify_notification_show(n, 0))
    {
        NCShared::LogFileError("Failed to show notification");
        return "Added new task but failed to show notification";
    }

    return "Added new task.";
}

std::string TaskUpdate(int id, std::string updatedTask, std::string newTaskDue)
{
    NCShared::LogFileMessage("Update task with id: " + std::to_string(id));

    std::ifstream tasksFile = OpenTaskFileRead();
    if (!tasksFile.is_open())
        return "Failed to open file for read, check /tmp/notecross.log for more details";

    json taskData = json::parse(tasksFile);
    tasksFile.close();
    NCShared::LogFileMessage("Parsed and closed tasksFile");

    if (!taskData.contains("tasks"))
    {
        NCShared::LogFileMessage("No 'tasks' array found in file, aborting...");
        return "No tasks found, did you already add a task?";
    }

    bool found = false;
    for (auto& task : taskData["tasks"])
    {
        if (task.contains("id") && task["id"] == id)
        {
            found = true;

            // Update the task text
            task["task"] = updatedTask;

            // Update the due date - handle "No due set" vs a numeric timestamp
            if (!newTaskDue.empty())
			{
                int unixDueDate = TaskDueToUnixTime(newTaskDue);

                if (unixDueDate == -1)
                {
                    return "Failed to parse due date, check `/tmp/notecross.log for more info and "
                           "check "
                           "GitHub for correct format";
                }
                task["due date"] = unixDueDate;
            }
            break;
        }
    }
    if (!found)
    {
        NCShared::LogFileMessage("No task found with id: " + std::to_string(id));
        return "Task with id " + std::to_string(id) + " not found";
    }

    std::ofstream tasksFileWrite = OpenTaskFileWrite();
    if (!tasksFileWrite.is_open())
        return "Failed to openfile, check /tmp/notecross.log for more details";
    tasksFileWrite << taskData.dump(4);

    tasksFile.close();

    NCShared::LogFileMessage("Updated task with id: " + std::to_string(id) +
                             " updated task: " + updatedTask);

    // NOTIFICATION
    std::string notificationText = "Task Upded with id: " + std::to_string(id);
    notify_init(notificationText.c_str());
    NotifyNotification* n = notify_notification_new(updatedTask.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000); // 5 seconds

    if (!notify_notification_show(n, 0))
    {
        NCShared::LogFileError("Failed to show notification");
        return "Updated new task but failed to show notification";
    }

    return "Updated task with id: " + std::to_string(id);
}

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

    NCShared::LogFileMessage("Removed task with id: " + std::to_string(id));

    // NOTIFICATION
    notify_init("Task Removed");
    std::string message = "Removed task with id: " + std::to_string(id);
    NotifyNotification* n = notify_notification_new(message.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000);

    if (!notify_notification_show(n, 0))
    {
        NCShared::LogFileError("Failed to show notification");
        return "Added new task but failed to show notification";
    }
    return "Removed task with id: " + std::to_string(id);
}
std::string TaskSync();
} // namespace NCShared
