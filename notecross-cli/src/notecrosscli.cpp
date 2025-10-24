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
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = "LIST";
    write(fileDiscriptor, msg.c_str(), msg.size());

    char response[256];
    int n = read(fileDiscriptor, response, sizeof(response) - 1);
    response[n] = '\0';

    std::cout << "--Current tasks--\n" << response << std::endl;

    close(fileDiscriptor);
}

void AddTask(std::string newTask)
{
    std::cout << "Adding task..\n";
    int fileDiscriptor = OpenSocket();

    if (fileDiscriptor == -1)
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = std::string("ADD") + "|" + newTask;
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
    int fileDiscriptor = OpenSocket();

    if (fileDiscriptor == -1)
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = "UPDATE";
    write(fileDiscriptor, msg.c_str(), msg.size());

    close(fileDiscriptor);
}

void RemoveTask(int id)
{
    std::cout << "Remove task..\n";
    int fileDiscriptor = OpenSocket();

    if (fileDiscriptor == -1)
        std::cerr << "Failed to open socket to NoteCross daemon.";

    std::string msg = "REMOVE";
    write(fileDiscriptor, msg.c_str(), msg.size());

    close(fileDiscriptor);
}

} // namespace NoteCrossCLI
