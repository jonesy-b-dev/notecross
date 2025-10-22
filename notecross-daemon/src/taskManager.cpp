#include "Task.h"
#include "log.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>

#define TASKDIR "~/.notecross/"

namespace Daemon
{

std::filesystem::path TaskFilePath()
{
#if defined(_WIN32)
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif
    return std::filesystem::path(home + std::string("/.notecross/tasks.json"));
}

int CreateTaskFile()
{
    if (!std::filesystem::create_directories(TaskFilePath()))
    {
        Daemon::LogError("Failed to create task file!");
        return 0;
    }
    return 1;
}

std::string TaskGetAll()
{
    if (!std::filesystem::exists(TaskFilePath()))
    {
        if (!CreateTaskFile())
        {
            return "Failed to create task file!";
        }
    }

    std::ifstream tasksFile(TaskFilePath());
    if (!tasksFile)
        Daemon::LogError("Failed to open tasks.json file: " + std::string(TaskFilePath()));

    // Read entire file into a string
    Daemon::LogMessage("Alive before resuly format");
    std::string result =
        std::string(std::istreambuf_iterator<char>(tasksFile), std::istreambuf_iterator<char>());
    Daemon::LogMessage("Alive after resuly format");
    return result;
}

std::string TaskAdd(Task newTask);
std::string TaskUpdate(int id, Task updatedTask);
std::string TaskRemove(int id);
std::string TaskSync();
} // namespace Daemon
