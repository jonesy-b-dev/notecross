#pragma once
#include "Task.h"
#include <string>

namespace Daemon
{
std::string TaskGetAll();
std::string TaskGetAllFormatted();
std::string TaskAdd(std::string newTask);
std::string TaskUpdate(int id, Task updatedTask);
std::string TaskRemove(int id);
std::string TaskSync();
} // namespace Daemon
