#pragma once
#include "../Task.h"
#include <string>

namespace NCShared
{
std::string TaskGetAll();
std::string TaskGetAllFormatted();
std::string TaskAdd(std::string newTask, std::string taskDue);
std::string TaskUpdate(int id, Task updatedTask);
std::string TaskRemove(int id);
std::string TaskSync();
} // namespace NCShared
