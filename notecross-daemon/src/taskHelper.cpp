#include "taskHelper.hpp"
#include "json.hpp"
#include "log.hpp"
#include <chrono>
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

int TaskDueToUnixTime(std::string taskDue)
{
    size_t sizeCharPos = taskDue.find_first_not_of("0123456789");
    if (sizeCharPos == std::string::npos)
    {
        return -1;
    }
    std::string amountStr = taskDue.substr(0, sizeCharPos);
    std::string sizeStr = taskDue.substr(sizeCharPos, 1);

    Daemon::LogMessage("amount: " + amountStr + " size: " + sizeStr);

    int amount;
    try
    {
        amount = std::stoi(amountStr);
    }
    catch (const std::invalid_argument& e)
    {
        Daemon::LogError("Invalid due input: not a number");
        return -1;
    }
    catch (const std::out_of_range& e)
    {
        Daemon::LogError("Due number out of range for int");
        return -1;
    }

    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::duration duration = now.time_since_epoch();
    auto epoch_time = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    Daemon::LogMessage("Current Unix timestamp: " + std::to_string(epoch_time));
    return 1;
}
} // namespace Daemon
