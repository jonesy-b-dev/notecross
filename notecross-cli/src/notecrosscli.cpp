#include "notecrosscli.hpp"
#include "socketHandler.hpp"
#include <iostream>
#include <unistd.h>

namespace NoteCrossCLI
{

void ListTask()
{
    int fileDiscriptor = OpenSocket();
	
    if (fileDiscriptor == -1)
    {
        std::cerr << "Failed to open socket to NoteCross daemon.";
    }
    std::string msg = "LIST";
    write(fileDiscriptor, msg.c_str(), msg.size());

	char buffer[256];
	int n = read(fileDiscriptor, buffer, sizeof(buffer) - 1);
	buffer[n] = '\0';
	std::cout << "Daemon says: " << buffer << std::endl;

    close(fileDiscriptor);
}
void AddTask(std::string newTask)
{
    std::cout << "Adding task..\n";
    int fileDiscriptor = OpenSocket();

    if (fileDiscriptor == -1)
    {
        std::cerr << "Failed to open socket to NoteCross daemon.";
    }

    std::string msg = "ADD";
    write(fileDiscriptor, msg.c_str(), msg.size());

    close(fileDiscriptor);
}
void UpdateTask(int id, std::string updatedTask) { std::cout << "Update task..\n"; }
void RemoveTask(int id) { std::cout << "Remove task..\n"; }

} // namespace NoteCrossCLI
