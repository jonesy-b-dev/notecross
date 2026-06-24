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
					"Notecross is a tool to help you manage to do tasks and sync them between devices\n\n"
					"These are the availaible cli options:\n"
                     "--add / -a {Task Name} {Task Due}\n"
                     "\tAdd a new task to your list\n\n"

                     "--update / -u {Task ID} {New Task Name} {New Task Due}\n"
                     "\tUpdate an existing task using the task ID\n\n"

                     "--remove / -r {Task ID}\n"
                     "\tRemove task using the task ID\n\n"

                     "--done / -d {Task ID}\n"
                     "\tMark task as done (not removing it) using the task ID\n\n"

                     "--list / -l\n"
                     "\tList all the tasks that are open right now\n\n"

                     "--sync / -s\n"
                     "\tManually sync the task file with your other devices\n\n"

                     "--help / -h\n"
                     "\tProvide this message\n";
    }
    else
    {
        std::cout << "No valid option given, `notecross help` to list options\n";
    }

    return 0;
}
