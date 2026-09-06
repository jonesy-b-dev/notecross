#include "include/taskManager.hpp"
#include "include/log.hpp"
#include "include/task.hpp"
#include "include/taskHelper.hpp"
#include <algorithm>
// #include <glib-2.0/glib.h">
#include <chrono>
#include <libnotify/notify.h>


namespace NCShared
{
std::string TaskGetAllFormatted(bool includeCompletedTasks)
{
	std::vector<Task> tasks = NCShared::GetAllTasks();

    if (tasks.empty())
        return "No tasks found.";

    if (!includeCompletedTasks)
    {
		std::vector<Task> filtered;

        for (const Task& task : tasks)
        {
            if (task.completed == false)
                filtered.push_back(task);
        }
        tasks = filtered;
    }

    // Find longest description
    size_t maxDesc = 0;
    for (const Task& task : tasks)
        maxDesc = std::max(maxDesc, task.description.size());

    std::ostringstream output;

    output << "== Current tasks ==\n";
    for (const Task& task : tasks)
    {
        int id = task.id;
        std::string desc = task.description;
        std::string due = task.dueDate == 0 ? "No due set" : NCShared::TaskDueToDate(task.dueDate);

		// Otherwise it shifts at id 10, will happen at 100 to, but dont care now
	    size_t descWidth = id < 10 ? maxDesc + 1 : maxDesc;

        output << "  " << id << ". " << std::left << std::setw(descWidth) << desc << " | Due: " << due
               << "\n";
    }

    NCShared::LogFileMessage("Listed all tasks.");

    return output.str();
}

std::string TaskAdd(std::string newTask, std::string taskDue)
{
    NCShared::LogFileMessage("Adding new task....");

    std::vector<Task> tasks = NCShared::GetAllTasks();

    Task newTaskObj;

    newTaskObj.id = tasks.empty() ? 0 : tasks.back().id + 1;
    newTaskObj.description = newTask;

    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::duration duration = now.time_since_epoch();
    auto currentUnixTime = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    newTaskObj.creationDate = currentUnixTime;

    if (taskDue.empty())
        newTaskObj.dueDate = 0;
    else
    {
        long unixDueDate = TaskDueToUnixTime(taskDue);

        if (unixDueDate == -1)
            return "Failed to parse due date, check `/tmp/notecross.log for more info and check "
                   "GitHub for correct format";

        newTaskObj.dueDate = unixDueDate;
    }
	newTaskObj.completed = false;

	tasks.push_back(newTaskObj);

	NCShared::WriteTasksToFile(tasks);

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

    std::vector<Task> tasks = NCShared::GetAllTasks();

    if (tasks.empty())
	{
        NCShared::LogFileMessage("No 'tasks' array found in file, aborting...");
        return "No tasks found, did you already add a task?";
	}

    Task updatedTaskObj;

    bool found = false;
    for (Task& task : tasks)
    {
        if (task.id == id)
        {
            found = true;

            task.description = updatedTask;

            if (!newTaskDue.empty())
            {
                long unixDueDate = TaskDueToUnixTime(newTaskDue);

                if (unixDueDate == -1)
                {
                    return "Failed to parse due date, check `/tmp/notecross.log for more info and "
                           "check "
                           "GitHub for correct format";
                }
                task.dueDate = unixDueDate;
            }
            break;
        }
    }
    if (!found)
    {
        NCShared::LogFileMessage("No task found with id: " + std::to_string(id));
        return "Task with id " + std::to_string(id) + " not found";
    }

	NCShared::WriteTasksToFile(tasks);

    NCShared::LogFileMessage("Updated task with id: " + std::to_string(id) +
                             " updated task: " + updatedTask);

    // NOTIFICATION
    std::string notificationText = "Task Updated with id: " + std::to_string(id);
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
    std::vector<Task> tasks = NCShared::GetAllTasks();

    if (tasks.empty())
	{
        NCShared::LogFileMessage("No 'tasks' array found in file, aborting...");
        return "No tasks found, did you already add a task?";
	}

    auto newEnd =
        std::remove_if(tasks.begin(),
                       tasks.end(),
                       [&id](const Task& task) { return task.id == id; });

	if (newEnd == tasks.end())
	    return "No task found with that id.";

    tasks.erase(newEnd, tasks.end());

	NCShared::WriteTasksToFile(tasks);

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
std::string TaskComplete(int id)
{
    NCShared::LogFileMessage("Completing task with id: " + std::to_string(id));

    std::vector<Task> tasks = NCShared::GetAllTasks();

    if (tasks.empty())
	{
        NCShared::LogFileMessage("No 'tasks' array found in file, aborting...");
        return "No tasks found, did you already add a task?";
	}
    bool found = false;
    for (Task& task : tasks)
    {
        if (task.id == id)
        {
            found = true;

            task.completed = true;

            break;
        }
    }
    if (!found)
    {
        NCShared::LogFileMessage("No task found with id: " + std::to_string(id));
        return "Task with id " + std::to_string(id) + " not found";
    }

	NCShared::WriteTasksToFile(tasks);

    NCShared::LogFileMessage("Completed task with id: " + std::to_string(id));

    // NOTIFICATION
    notify_init("Task Completed");
    std::string message = "Completed task with id: " + std::to_string(id);
    NotifyNotification* n = notify_notification_new(message.c_str(), " ", 0);
    notify_notification_set_timeout(n, 5000);

    if (!notify_notification_show(n, 0))
    {
        NCShared::LogFileError("Failed to show notification");
        return "Added new task but failed to show notification";
    }
    return "Completed task with id: " + std::to_string(id);
}
std::string TaskSync();
} // namespace NCShared
