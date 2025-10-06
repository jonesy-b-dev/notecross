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
	std::string option = argv[1];

	if (option == "--add" || option == "-a")
	{
		NoteCrossCLI::AddTask(argv[2]);
	}
	else if (option == "--update" || option == "-u")
	{
		NoteCrossCLI::UpdateTask(argv[2], argv[3]);
	}
	else if (option == "--remove" || option == "-r")
	{
		NoteCrossCLI::RemoveTask(argv[2]);
	}
	else if (option == "--list" || option == "-l")
	{
		NoteCrossCLI::ListTask();
	}
	else if (option == "--sync" || option == "-s")
	{
		NoteCrossCLI::AddTask();
	}

    std::cout << "sup bitches v2\n";
}
