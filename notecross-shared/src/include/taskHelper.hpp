#pragma once
#include <filesystem>

namespace NCShared
{
std::filesystem::path TaskFilePath(bool withFile);
int CreateTaskFile();
std::ifstream OpenTaskFileRead();
std::ofstream OpenTaskFileWrite();
int TaskDueToUnixTime(std::string taskDue);
} // namespace NCShared
