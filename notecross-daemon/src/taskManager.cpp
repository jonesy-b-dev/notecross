#include "Task.h"
#include "log.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

#define TASKPATH "~/Notecross/tasks.json"
namespace Daemon
{
std::string TaskGetAll()
{
    if (!std::filesystem::exists(TASKPATH))
    {
        std::string result =
            std::string("No task file exists (yet), add a task first to create the file");
        Daemon::LogMessage(result);
        return result;
    }

    std::ifstream tasksFile(TASKPATH);

    if (!tasksFile)
    {
        Daemon::LogError("Failed to open tasks.json file: " + std::string(TASKPATH));
    }

    // Read entire file into a string
    return std::string(std::istreambuf_iterator<char>(tasksFile), std::istreambuf_iterator<char>());
}

void TaskAdd(Task newTask);
void TaskUpdate(int id, Task updatedTask);
void TaskRemove(int id);
void TaskSync();
} // namespace Daemon
