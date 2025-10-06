#pragma once
#include <string>

namespace NoteCrossCLI
{
void AddTask(std::string newTask);
void UpdateTask(int id, std::string updatedTask);
void RemoveTask(int id);
void ListTask();
}
