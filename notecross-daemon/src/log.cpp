#include "log.hpp"
#include <chrono>
#include <iostream>

void LogMessage(std::string message)
{
	auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm = *std::localtime(&t); // convert to local time

    // journalctl-style formatting: "Jul 31 01:26:02"
    std::cout << "[" << std::put_time(&local_tm, "%b %d %H:%M:%S") << "] " << message << "\n";
}
void LogError(std::string error)
{
	auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm = *std::localtime(&t); // convert to local time

    // journalctl-style formatting: "Jul 31 01:26:02"
    std::cout << "[" << std::put_time(&local_tm, "%b %d %H:%M:%S") << "] " << error << "\n";
}
