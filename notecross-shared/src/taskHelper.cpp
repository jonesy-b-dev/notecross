#include "include/taskHelper.hpp"
#include "json.hpp"
#include "log.hpp"
#include <chrono>
#include <ctime>
#include <fstream>

using json = nlohmann::json;
namespace NCShared
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
    NCShared::LogMessage("Creating tasks file");
    std::filesystem::create_directories(TaskFilePath(false));

    std::ofstream tasksFile;
    tasksFile.open(TaskFilePath(true).string(), std::ios::out | std::ios::trunc);
    if (!tasksFile.is_open())
    {
        NCShared::LogError("Failed to open task file for initialization!");
        return 0;
    }

    json taskData;
    taskData["tasks"] = json::array();

    std::string content = taskData.dump(4);
    NCShared::LogMessage("JSON content: " + content);
    tasksFile << content;
    tasksFile.flush(); // Force write to disk
    NCShared::LogMessage("Initiazed task file");

    tasksFile.close();

    return 1;
}

std::ifstream OpenTaskFileRead()
{
    if (!std::filesystem::exists(TaskFilePath(true)))
        if (!CreateTaskFile())
        {
            NCShared::LogError("Failed to create task file!");
            return {};
        }

    std::ifstream tasksFile(TaskFilePath(true));
    if (!tasksFile.is_open())
        NCShared::LogError("Failed to open tasks.json file: " + std::string(TaskFilePath(true)));

    return tasksFile;
}

std::ofstream OpenTaskFileWrite()
{
    std::ofstream tasksFile;
    if (!std::filesystem::exists(TaskFilePath(true)))
        if (!CreateTaskFile())
        {
            NCShared::LogError("Failed to create task file!");
            return tasksFile;
        }

    tasksFile.open(TaskFilePath(true));
    if (!tasksFile)
        NCShared::LogError("Failed to open tasks.json file: " + std::string(TaskFilePath(true)));

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

    NCShared::LogMessage("amount: " + amountStr + " size: " + sizeStr);

    int amount;
    try
    {
        amount = std::stoi(amountStr);
    }
    catch (const std::invalid_argument& e)
    {
        NCShared::LogError("Invalid due input: not a number");
        return -1;
    }
    catch (const std::out_of_range& e)
    {
        NCShared::LogError("Due number out of range for int");
        return -1;
    }

    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::duration duration = now.time_since_epoch();
    auto currentUnixTime = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    NCShared::LogMessage("Current Unix timestamp: " + std::to_string(currentUnixTime));

    int dueUnixTime = 0;
    if (sizeStr == "m")
    {
        return currentUnixTime + amount * 60;
    }
    else if (sizeStr == "h")
    {
        return currentUnixTime + amount * 60 * 60;
    }
    else if (sizeStr == "d")
    {
        return currentUnixTime + amount * 60 * 60 * 24;
    }
    return -1;
}

std::string TaskDueToDate(int taskDue)
{
    time_t now = time(nullptr);
    long diff = (long)(taskDue - now);

    bool negative = diff < 0;
    long abs_diff = negative ? -diff : diff;

    long days = abs_diff / 86400;
    long hours = (abs_diff % 86400) / 3600;
    long minutes = (abs_diff % 3600) / 60;
    long seconds = abs_diff % 60;

    std::ostringstream oss;
    if (negative)
        oss << "-";
    if (days > 0)
        oss << days << "d ";
    if (hours > 0)
        oss << hours << "h ";
    if (minutes > 0)
        oss << minutes << "m ";
    if (seconds > 0)
        oss << seconds << "s";

    std::string result = oss.str();
    if (!result.empty() && result.back() == ' ')
        result.pop_back();
    return result;
}
} // namespace NCShared
