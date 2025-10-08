#include "notecrosscli.hpp"
#include <iostream>

namespace NoteCrossCLI
{

void AddTask(std::string newTask) { std::cout << "Added task\n"; }
void UpdateTask(int id, std::string updatedTask) { std::cout << "Update task..\n"; }
void RemoveTask(int id) { std::cout << "Remove task..\n"; }
void ListTask() { std::cout << "Listing tasks..\n"; }

} // namespace NoteCrossCLI
