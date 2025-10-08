#include "./helpers/extras.hpp"
#include "notecrosscli.hpp"
#include <iostream>

// ARGUMENT ORDER
// INDEX 	| 	data
// -------------------
// 0		|	notecross
// 1		|	option
// 2 		| 	Task data (when no id is needed)
// 2		| 	id (when there is id needed)
// 3 		| 	Task data (when id is needed)

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "No arguments given, `notecross help` to list posible options\n";
        return 0;
    }

    std::string option = argv[1];

    if (option == "--add" || option == "-a")
    {
        if (argc < 3)
        {
            std::cerr << "Not enough arguments given, check `notecross --help` for more details.\n";
            return 0;
        }
        std::string taskData = argv[2];
        NoteCrossCLI::AddTask(taskData);
    }
    else if (option == "--update" || option == "-u")
    {
        if (argc < 4)
        {
            std::cerr << "Not enough arguments given, check `notecross --help` for more details.\n";
            return 0;
        }
        if (!isInteger(argv[2]))
        {
            std::cerr << "Invalid id, id is not an interger\n";
            return 0;
        }

        int id = std::stoi(argv[2]);
        std::string taskData = argv[3];
        NoteCrossCLI::UpdateTask(id, taskData);
    }
    else if (option == "--remove" || option == "-r")
    {
        if (argc < 2)
        {
            std::cerr << "Not enough arguments given, check `notecross --help` for more details.\n";
            return 0;
        }
        if (!isInteger(argv[2]))
        {
            std::cerr << "Invalid id, id is not an interger\n";
            return 0;
        }

        int id = std::stoi(argv[2]);
        NoteCrossCLI::RemoveTask(id);
    }
    else if (option == "--list" || option == "-l")
    {
        NoteCrossCLI::ListTask();
    }
    else if (option == "--sync" || option == "-s")
    {
        // NoteCrossCLI::AddTask();
    }
    else if (option == "--help" || option == "-h")
    {
        std::cout << "--Notecross help--\n\n"
                     "--add / -a {newTaskName} {newTaskDue}\n"
                     "\tAdd a new task using the AddTask function\n\n"

                     "--update / -u {taskId} {newTaskName} {newTaskDue}\n"
                     "\tUpdate task using the TaskUpdate function\n\n"

                     "--remove / -r {taskId}\n"
                     "\tRemove task using the TaskRemove function\n\n"

                     "--done / -d {taskId}\n"
                     "\tMark task as done using the TaskDone function\n\n"

                     "--list / -l\n"
                     "\tList all the tasks that are open using the TaskGetAll() function\n\n"

                     "--sync / -s\n"
                     "\tManually sync the task file using the TaskSync option\n\n"

                     "--help / -h\n"
                     "\tList all CLI options\n";
    }
    else
    {
        std::cout << "No valid option given, `notecross help` to list options\n";
    }
}
