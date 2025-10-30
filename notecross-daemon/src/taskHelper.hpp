#pragma once
#include <filesystem>

namespace Daemon
{
std::filesystem::path TaskFilePath(bool withFile);
int CreateTaskFile();
std::ifstream OpenTaskFileRead();
std::ofstream OpenTaskFileWrite();
int TaskDueToUnixTime(std::string taskDue);
} // namespace Daemon
