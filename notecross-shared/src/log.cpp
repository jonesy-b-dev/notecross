#include "include/log.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace NCShared
{
std::string FormatMessge(std::string_view message, bool isError);
std::filesystem::path LogFilePath();

void LogConsoleMessage(std::string_view message)
{
	std::cout << FormatMessge(message, false);
}

void LogConsoleError(std::string_view error)
{
	std::cout << FormatMessge(error, true);
}

void LogFileMessage(std::string_view message)
{
    std::ofstream logFile(LogFilePath());

    logFile << FormatMessge(message, false);

	logFile.close();
}

void LogFileError(std::string_view error)
{
    std::ofstream logFile(LogFilePath());

    logFile << FormatMessge(error, false);

	logFile.close();
}

std::string FormatMessge(std::string_view message, bool isError)
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm = *std::localtime(&t); // convert to local time

    std::ostringstream completeMessgage;

    // journalctl-style formatting: "Jul 31 01:26:02"
    if (isError)
    {
        completeMessgage << "[" << std::put_time(&local_tm, "%b %d %H:%M:%S") << "] [Error!]"
                         << message << "\n";
        return completeMessgage.str();
    }
    else
    {
        completeMessgage << "[" << std::put_time(&local_tm, "%b %d %H:%M:%S") << "] [Message]"
                         << message << "\n";
        return completeMessgage.str();
    }
}

std::filesystem::path LogFilePath()
{
#if defined(_WIN32)
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    return std::filesystem::path(home + std::string("/.notecross/notecross.log"));
}

} // namespace NCShared
