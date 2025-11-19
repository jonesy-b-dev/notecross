#pragma once
#include <string_view>

namespace NCShared
{
void LogMessage(std::string_view message);
void LogError(std::string_view error);
} // namespace NCShared
