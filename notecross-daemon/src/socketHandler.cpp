#include "socketHandler.hpp"
#include "log.hpp"
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace Daemon
{
int OpenSocket()
{
    struct sockaddr_un serverSockAddr{};

    int socketFileDiscriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    Daemon::LogMessage(
        "Notecross daemon socket file discriptor: " + std::to_string(socketFileDiscriptor) + "\n");
    if (socketFileDiscriptor == -1)
    {
        Daemon::LogError("Failed to open daemon server socket!");
        close(socketFileDiscriptor);
        exit(1);
    }

    if (strlen(SERVER_SOCK_PATH) > sizeof(serverSockAddr.sun_path) - 1)
    {
        LogError(std::string("Server socket path too long: ") + SERVER_SOCK_PATH);
        close(socketFileDiscriptor);
        exit(1);
    }

    // empty sock path to avoid conflics
    // if (remove(SERVER_SOCK_PATH) == -1 && errno != ENOENT)
    //{
    //    LogError(std::string("Failed to remove ") + SERVER_SOCK_PATH + std::string(" ") +
    //             std::to_string(errno));
    //    close(socketFileDiscriptor);
    //    exit(1);
    //}

    if (unlink(SERVER_SOCK_PATH) == -1 && errno != ENOENT)
    {
        LogError(std::string("Failed to remove old socket file: ") + strerror(errno));
        close(socketFileDiscriptor);
        exit(1);
    }

    serverSockAddr.sun_family = AF_UNIX;
    std::strncpy(serverSockAddr.sun_path, SERVER_SOCK_PATH, sizeof(serverSockAddr.sun_path) - 1);

    socklen_t len = offsetof(struct sockaddr_un, sun_path) + strlen(serverSockAddr.sun_path) + 1;
    if (bind(socketFileDiscriptor, (struct sockaddr*)&serverSockAddr, len) == -1)
    {
        LogError("Bind failed: " + std::string(strerror(errno)) + "\n");
        close(socketFileDiscriptor);
        exit(1);
    }

    LogMessage("Opened socket at: " + std::string(SERVER_SOCK_PATH));

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
        int conectionFileDiscription = accept(socketFileDiscriptor, NULL, NULL);
        LogError("Accepted socket, file discriptor = " + std::to_string(conectionFileDiscription));
    }
}
} // namespace Daemon
