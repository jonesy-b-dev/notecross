#pragma once
#include "Task.h"
#include <string>

namespace Daemon
{
std::string TaskGetAll();
std::string TaskAdd(Task newTask);
std::string TaskUpdate(int id, Task updatedTask);
std::string TaskRemove(int id);
std::string TaskSync();
} // namespace Daemon
