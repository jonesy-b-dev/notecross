#include "notecrosscli.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        std::string argument = argv[i];

        if (argument == "--add" || argument == "-a")
        {
            std::cout << argv[i] << std::endl;
            NoteCrossCLI::AddTask();
        }
    }

    std::cout << "sup bitches v2\n";
}
