#include "socketHandler.hpp"
#include "log.hpp"
#include <cstring>
#define SOCK_PATH "/tmp/NoteCrossDaemonSocket"
#include "taskManager.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace Daemon
{
int OpenSocket()
{
    struct sockaddr_un serverSockAddr{};

    int socketFileDiscriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    Daemon::LogMessage("Notecross daemon socket file discriptor: " +
                       std::to_string(socketFileDiscriptor));
    if (socketFileDiscriptor == -1)
    {
        Daemon::LogError("Failed to open daemon server socket!");
        close(socketFileDiscriptor);
        exit(1);
    }

    if (strlen(SOCK_PATH) > sizeof(serverSockAddr.sun_path) - 1)
    {
        LogError(std::string("Server socket path too long: ") + SOCK_PATH);
        close(socketFileDiscriptor);
        exit(1);
    }

    // empty sock path to avoid conflics
    // if (remove(SOCK_PATH) == -1 && errno != ENOENT)
    //{
    //    LogError(std::string("Failed to remove ") + SOCK_PATH + std::string(" ") +
    //             std::to_string(errno));
    //    close(socketFileDiscriptor);
    //    exit(1);
    //}

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

        if (strcmp(buffer, "ADD") == 0)
        {
            LogMessage("Recieved ADD request");
            // Daemon::TaskAdd();
        }
        if (strcmp(buffer, "LIST") == 0)
        {
            Daemon::LogMessage("Recieved LIST request");
            std::string result = Daemon::TaskGetAll();
            Daemon::LogMessage("Alive before print");
            Daemon::LogMessage(result.c_str());
            Daemon::LogMessage("Alive before write");
            write(client, result.c_str(), 4);
            Daemon::LogMessage("Alive after write");
        }

        Daemon::LogMessage("Alive before close");
        close(client);
        Daemon::LogMessage("Alive after close");
    }
}
} // namespace Daemon
