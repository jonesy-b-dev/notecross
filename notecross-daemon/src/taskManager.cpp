#include "Task.h"
#include "log.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>
#include "taskHelper.hpp"

#define TASKDIR "~/.notecross/"

namespace Daemon
{
std::string TaskGetAll()
{
	std::ifstream tasksFile = OpenTaskFile();
	if(!tasksFile.is_open())
	{
		return "Failed to openfile, check /tmp/notecross.log for more details";
	}
    // Read entire file into a string
    std::string result =
        std::string(std::istreambuf_iterator<char>(tasksFile), std::istreambuf_iterator<char>());
    return result;
}

std::string TaskAdd(Task newTask)
{
    if (!std::filesystem::exists(TaskFilePath(true)))
        if (!CreateTaskFile())
            return "Failed to create task file!";

    std::ifstream tasksFile(TaskFilePath(true));
    if (!tasksFile)
        Daemon::LogError("Failed to open tasks.json file: " + std::string(TaskFilePath(true)));
}
std::string TaskUpdate(int id, Task updatedTask);
std::string TaskRemove(int id);
std::string TaskSync();
} // namespace Daemon
