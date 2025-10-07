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
    int id = isInteger(argv[2]) ? std::stoi(argv[2]) : -1;
    std::string taskData = (id == -1) ? argv[2] : argv[3];

    if (option == "--add" || option == "-a")
    {
        NoteCrossCLI::AddTask(taskData);
    }
    else if (option == "--update" || option == "-u")
    {
        NoteCrossCLI::UpdateTask(id, taskData);
    }
    else if (option == "--remove" || option == "-r")
    {
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
    else
    {
        std::cout << "No option given, `notecross help` to list options\n";
    }
}
