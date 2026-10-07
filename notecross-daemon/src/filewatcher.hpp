#pragma once
#include <sys/inotify.h>
#include <filesystem>

int initializeFileWatcher(std::filesystem::path taskFile);
