#pragma once
#include <vector>
#include <filesystem>
#include "task.hpp"

namespace NCShared
{
int CreateTaskFile();
int TaskDueToUnixTime(std::string taskDue);
std::string TaskDueToDate(int taskDue);

// API
std::vector<Task> GetAllTasks();
bool WriteTasksToFile(std::vector<Task>& incommingTasks);
} // namespace NCShared
