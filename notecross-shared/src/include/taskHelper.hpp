#pragma once
#include <filesystem>
#include "../json.hpp"

namespace NCShared
{
std::filesystem::path TaskFilePath(bool withFile);
int CreateTaskFile();
nlohmann::json OpenTaskFileRead();
std::ofstream OpenTaskFileWrite();
int TaskDueToUnixTime(std::string taskDue);
std::string TaskDueToDate(int taskDue);
} // namespace NCShared
