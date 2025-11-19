#pragma once
#include <string_view>

namespace NCDaemon
{
void LogMessage(std::string_view message);
void LogError(std::string_view error);
} // namespace NCDaemon
