#include "socketHandler.hpp"
#include "log.hpp"
#include <cstring>
#include <stdexcept>
#define SOCK_PATH "/tmp/NoteCrossDaemonSocket"
// #include <taskManager.hpp>
#include "../../notecross-shared/src/include/taskManager.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace NCDaemon
{
int OpenSocket()
{
    struct sockaddr_un serverSockAddr{};

    int socketFileDiscriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    NCDaemon::LogMessage("Notecross daemon socket file discriptor: " +
                         std::to_string(socketFileDiscriptor));
    if (socketFileDiscriptor == -1)
    {
        NCDaemon::LogError("Failed to open daemon server socket!");
        close(socketFileDiscriptor);
        exit(1);
    }

    if (strlen(SOCK_PATH) > sizeof(serverSockAddr.sun_path) - 1)
    {
        LogError(std::string("Server socket path too long: ") + SOCK_PATH);
        close(socketFileDiscriptor);
        exit(1);
    }

    if (unlink(SOCK_PATH) == -1 && errno != ENOENT)
    {
        LogError(std::string("Failed to remove old socket file: ") + strerror(errno));
        close(socketFileDiscriptor);
        exit(1);
    }

    serverSockAddr.sun_family = AF_UNIX;
    std::strncpy(serverSockAddr.sun_path, SOCK_PATH, sizeof(serverSockAddr.sun_path) - 1);

    socklen_t len = offsetof(struct sockaddr_un, sun_path) + strlen(serverSockAddr.sun_path) + 1;
    if (bind(socketFileDiscriptor, (struct sockaddr*)&serverSockAddr, len) == -1)
    {
        LogError("Bind failed: " + std::string(strerror(errno)) + "\n");
        close(socketFileDiscriptor);
        exit(1);
    }

    LogMessage("Opened socket at: " + std::string(SOCK_PATH));

    if (listen(socketFileDiscriptor, 5) == -1)
    {
        LogError("Failed to start listening!");
        close(socketFileDiscriptor);
        exit(1);
    }
    return socketFileDiscriptor;
}

void HandleConnections(int socketFileDiscriptor)
{
    while (1)
    {
        LogMessage("Listing for connections..");
        // Blocks until a connection is active
        int client = accept(socketFileDiscriptor, NULL, NULL);
        LogError("Accepted socket, file discriptor = " + std::to_string(client));

        char buffer[128];
        int n = read(client, buffer, sizeof(buffer) - 1);
        buffer[n] = '\0';

        std::string strBuffer = buffer;
        std::string data;
        std::string option;
        std::string due;

        size_t pipePos = strBuffer.find("|");
        size_t dashPos = strBuffer.find("-");
        if (pipePos == std::string::npos)
        {
            option = strBuffer;
        }
        else
        {
            option = strBuffer.substr(0, pipePos);
            if (dashPos == std::string::npos)
            {
                data = strBuffer.substr(pipePos + 1, strBuffer.length());
            }
            else
            {
                data = strBuffer.substr(pipePos + 1, dashPos - pipePos - 1);
                due = strBuffer.substr(dashPos + 1, strBuffer.length());
            }
        }

        NCDaemon::LogMessage(option + " | " + data + " - " + due);

        if (strcmp(option.c_str(), "ADD") == 0)
        {
            LogMessage("Recieved ADD request");
            std::string result = NCShared::TaskAdd(data, due);
            write(client, result.c_str(), result.size());
        }
        if (strcmp(option.c_str(), "LIST") == 0)
        {
            LogMessage("Recieved LIST request");
            std::string result = NCShared::TaskGetAllFormatted();
            write(client, result.c_str(), result.size());
        }
        if (strcmp(option.c_str(), "REMOVE") == 0)
        {
            NCDaemon::LogMessage("Remove data: " + data);
            int id;
            try
            {
                id = std::stoi(data);
            }
            catch (const std::invalid_argument& e)
            {
                NCDaemon::LogError("Invalid input: not a number");
                std::string result = "Failed to convert number to int, got std::invalid_argument";
                write(client, result.c_str(), result.size());
                continue;
            }
            catch (const std::out_of_range& e)
            {
                NCDaemon::LogError("Number out of range for int");
                std::string result = "Failed to convert number to int, got std::out_of_range";
                write(client, result.c_str(), result.size());
                continue;
            }
            std::string result = NCShared::TaskRemove(id);
            write(client, result.c_str(), result.size());
        }
        close(client);
    }
}
} // namespace NCDaemon
