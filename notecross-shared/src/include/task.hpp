#pragma once
#include <string>

namespace NCShared
{
struct Task
{
    int id;
    std::string description;
    int creationDate;
    long dueDate;
    bool completed;
};
} // namespace NCShared
