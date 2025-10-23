#include "taskHelper.hpp"
#include "log.hpp"
#include <fstream>

namespace Daemon
{
std::filesystem::path TaskFilePath(bool withFile)
{
#if defined(_WIN32)
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    return withFile ? std::filesystem::path(home + std::string("/.notecross/tasks.json"))
                    : std::filesystem::path(home + std::string("/.notecross/"));
}

int CreateTaskFile()
{
    if (!std::filesystem::create_directories(TaskFilePath(false)))
    {
        Daemon::LogError("Failed to create task file!");
        return 0;
    }
    std::ofstream(TaskFilePath(true));
    return 1;
}

std::ifstream OpenTaskFile()
{
    std::ifstream tasksFile;
    if (!std::filesystem::exists(TaskFilePath(true)))
        if (!CreateTaskFile())
        {
            Daemon::LogError("Failed to create task file!");
            return tasksFile;
        }

    tasksFile.open(TaskFilePath(true));
    if (!tasksFile)
        Daemon::LogError("Failed to open tasks.json file: " + std::string(TaskFilePath(true)));

    return tasksFile;
}
} // namespace Daemon
