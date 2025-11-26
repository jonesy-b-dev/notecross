#include "notecrosscli.hpp"
#include "socketHandler.hpp"
#include <iostream>
#include <unistd.h>

namespace NCCLI
{
void ListTask()
{
    int fileDiscriptor = NCCLI::OpenSocket();

    if (fileDiscriptor == -1)
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = "LIST";
    write(fileDiscriptor, msg.c_str(), msg.size());

    char response[256];
    int n = read(fileDiscriptor, response, sizeof(response) - 1);
    response[n] = '\0';

    std::cout << "== Current tasks ==\n" << response << std::endl;

    close(fileDiscriptor);
}

void AddTask(std::string newTask, std::string taskDue)
{
    std::cout << "Adding task..\n";
    int fileDiscriptor = NCCLI::OpenSocket();

    if (fileDiscriptor == -1)
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = std::string("ADD") + "|" + newTask + "-" + taskDue;
    write(fileDiscriptor, msg.c_str(), msg.size());

    char response[256];
    int n = read(fileDiscriptor, response, sizeof(response) - 1);
    response[n] = '\0';

    std::cout << response << std::endl;

    close(fileDiscriptor);
}

void UpdateTask(int id, std::string updatedTask)
{
    std::cout << "Update task..\n";
    int fileDiscriptor = NCCLI::OpenSocket();

    if (fileDiscriptor == -1)
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = std::string("UPDATE") + "|" + updatedTask;
    write(fileDiscriptor, msg.c_str(), msg.size());

    close(fileDiscriptor);
}

void RemoveTask(int id)
{
    std::cout << "Remove task..\n";
    int fileDiscriptor = NCCLI::OpenSocket();

    if (fileDiscriptor == -1)
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = std::string("REMOVE") + "|" + std::to_string(id);
    write(fileDiscriptor, msg.c_str(), msg.size());

    close(fileDiscriptor);
}

} // namespace NCCLI
