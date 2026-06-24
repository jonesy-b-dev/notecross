#include "helpers/extras.hpp"
#include "log.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

bool isInteger(const char* s)
{
    if (s == nullptr || *s == '\0')
        return false;
    if (!std::isdigit(*s) && *s != '-' && *s != '+')
        return false;

    char* p;
    std::strtol(s, &p, 10);
    return (*p == '\0');
}

int ParseId(const char* id)
{
    if (!isInteger(id))
    {
        NCShared::LogFileError("Invalid id, " + std::string(id) + " is not an integer!");
        std::cerr << "Invalid id, id is not an interger\n";
        return -1;
    }

	int taskId;
    try
    {
        taskId = std::stoi(id);
    }
    catch (const std::out_of_range&)
    {
        NCShared::LogFileError("Id out of range: " + std::string(id));
        std::cerr << "Invalid id, value is too large\n";
        return -1;
    }
	return taskId;
}
