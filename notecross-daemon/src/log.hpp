#pragma once
#include <string>

namespace Daemon
{
void LogMessage(std::string_view message);
void LogError(std::string_view error);
} // namespace Daemon
