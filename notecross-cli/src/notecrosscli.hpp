#pragma once
#include <string>

namespace NCCLI
{
void AddTask(std::string newTask, std::string taskDue);
void UpdateTask(int id, std::string updatedTask);
void RemoveTask(int id);
void ListTask();
}
