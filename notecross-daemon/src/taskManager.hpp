#pragma once
#include "Task.h"
#include <string>

namespace Daemon
{
std::string TaskGetAll();
void TaskAdd(Task newTask);
void TaskUpdate(int id, Task updatedTask);
void TaskRemove(int id);
void TaskSync();
} // namespace Daemon
