#pragma once
#include <filesystem>

namespace Daemon
{
std::filesystem::path TaskFilePath(bool withFile);
int CreateTaskFile();
std::ifstream OpenTaskFile();
} // namespace Daemon
