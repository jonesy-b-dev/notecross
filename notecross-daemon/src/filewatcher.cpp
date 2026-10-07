#include "filewatcher.hpp"
#include <cstring>
#include <log.hpp>
#include <string>
#include <unistd.h>

int initializeFileWatcher(std::filesystem::path taskFile)
{
    int inotifyHandle = inotify_init();
    if (inotifyHandle < 0)
    {
		NCShared::LogFileError("Failed to initialize inotify");
        return 1;
    }

    std::string directoryPath = std::string(std::getenv("HOME")) + "/.notecross";
    std::string targetFileName = "tasks.json";

    int watchDescriptor = inotify_add_watch(inotifyHandle, directoryPath.c_str(), IN_MODIFY);

    if (watchDescriptor < 0)
    {
		NCShared::LogFileError("Failed to add watch: " + std::string(std::strerror(errno)));

        if (errno == ENOENT)
        {
			NCShared::LogFileError("File does not exist");
        }
        else if (errno == EACCES)
        {
			NCShared::LogFileError("Permission denied");
        }
        else if (errno == ENOSPC)
        {
			NCShared::LogFileError("Max inotify watches reached, increase fs.inotify.max_user_watches");
        }

        close(inotifyHandle);
        return 1;
    }
	return inotifyHandle;
}

