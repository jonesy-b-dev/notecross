#include "./helpers/extras.hpp"
#include <iostream>
#include <log.hpp>
#include <taskManager.hpp>

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
        std::cout << "No arguments given, `notecross --help` to list posible options\n";
        return 0;
    }

    std::string option = argv[1];

    if (option == "--add" || option == "-a")
    {
        NCShared::LogFileMessage("Recieved add request");
        std::string taskDue = "";

        if (argc < 3)
        {
            NCShared::LogFileError("Not enough arguments given for add request, stopping...");
            std::cerr << "Not enough arguments given, check `notecross --help` for more details.\n";
            return 0;
        }
        if (argc == 4) // Due date was given
        {
            taskDue = argv[3];
            NCShared::LogFileMessage("Due date was given to new task, value: " + taskDue);
        }
        std::string taskData = argv[2];

        NCShared::LogFileMessage("Data given to shared module: Task Data: " + taskData +
                                 " Task Due: " + taskDue);
        std::cout << NCShared::TaskAdd(taskData, taskDue);
    }
    else if (option == "--update" || option == "-u")
    {
        NCShared::LogFileMessage("Recieved update request");
        std::string taskDue = "";

        if (argc < 4)
        {
            NCShared::LogFileError("Not enough arguments given for add request, stopping...");
            std::cerr << "Not enough arguments given, check `notecross --help` for more details.\n";
            return 0;
        }

        int taskId = ParseId(argv[2]);
		if (taskId == -1)
			return 0;
        std::string taskData = argv[3];

        NCShared::LogFileMessage("Data given to shared module: Task Data: " + taskData +
                                 " Task Id: " + std::to_string(taskId));
        std::cout << NCShared::TaskUpdate(taskId, taskData, taskDue);
    }
    else if (option == "--remove" || option == "-r")
    {
        NCShared::LogFileMessage("Recieved remove request");
        if (argc < 3)
        {
            NCShared::LogFileError("Not enough arguments given for remove request, stopping...");
            std::cerr << "Not enough arguments given, check `notecross --help` for more details.\n";
            return 0;
        }
		int taskId = ParseId(argv[2]);
		if (taskId == -1)
			return 0;

        NCShared::LogFileMessage("Removing task with id: " + std::to_string(taskId));
        std::cout << NCShared::TaskRemove(taskId);
    }
    else if (option == "--list" || option == "-l")
    {
        NCShared::LogFileMessage("Recieved list request");

        NCShared::LogFileMessage("Listing current tasks");
        std::cout << NCShared::TaskGetAllFormatted();
    }
    else if (option == "--sync" || option == "-s")
    {
        // NCCLI::AddTask();
    }
    else if (option == "--help" || option == "-h")
    {
        NCShared::LogFileMessage("Recieved help request");

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

    return 0;
}
