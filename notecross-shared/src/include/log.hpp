#pragma once
#include <string_view>

namespace NCShared
{
void LogFileMessage(std::string_view message);
void LogFileError(std::string_view error);

void LogConsoleMessage(std::string_view message);
void LogConsoleError(std::string_view error);
} // namespace NCShared
