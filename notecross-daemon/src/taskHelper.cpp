#include "taskHelper.hpp"
#include "json.hpp"
#include "log.hpp"
#include <fstream>

using json = nlohmann::json;
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
    Daemon::LogMessage("Creating tasks file");
    std::filesystem::create_directories(TaskFilePath(false));

    std::ofstream tasksFile;
    tasksFile.open(TaskFilePath(true).string(), std::ios::out | std::ios::trunc);
    if (!tasksFile.is_open())
    {
        Daemon::LogError("Failed to open task file for initialization!");
        return 0;
    }

    json taskData;
    taskData["tasks"] = json::array();

    std::string content = taskData.dump(4);
    Daemon::LogMessage("JSON content: " + content);
    tasksFile << content;
    tasksFile.flush(); // Force write to disk
    Daemon::LogMessage("Initiazed task file");

    tasksFile.close();

    return 1;
}

std::ifstream OpenTaskFileRead()
{
    if (!std::filesystem::exists(TaskFilePath(true)))
        if (!CreateTaskFile())
        {
            Daemon::LogError("Failed to create task file!");
            return {};
        }

    std::ifstream tasksFile(TaskFilePath(true));
    if (!tasksFile.is_open())
        Daemon::LogError("Failed to open tasks.json file: " + std::string(TaskFilePath(true)));

    return tasksFile;
}

std::ofstream OpenTaskFileWrite()
{
    std::ofstream tasksFile;
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
