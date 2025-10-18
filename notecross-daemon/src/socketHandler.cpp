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
        Daemon::LogError("Failed to open daemon server socket!\n");
        exit(1);
    }

    if (strlen(SERVER_SOCK_PATH) > sizeof(serverSockAddr.sun_path) - 1)
    {
        LogError(std::string("Server socket path too long: ") + SERVER_SOCK_PATH);
        exit(1);
    }

    // empty sock path to avoid conflics
    if (remove(SERVER_SOCK_PATH) == -1 && errno != ENOENT)
    {
        LogError(std::string("Failed to remove ") + SERVER_SOCK_PATH);
        exit(1);
    }

    serverSockAddr.sun_family = AF_UNIX;
    std::strncpy(serverSockAddr.sun_path, SERVER_SOCK_PATH, sizeof(serverSockAddr.sun_path) - 1);

    if (bind(socketFileDiscriptor, (struct sockaddr*)&serverSockAddr, sizeof(serverSockAddr)) == -1)
    {
        LogError("Failed to bind server socket!\n");
        exit(1);
    }
    if (listen(socketFileDiscriptor, 5) == -1)
    {
        LogError("Failed to start listening!");
        exit(1);
    }

    close(socketFileDiscriptor);
    return socketFileDiscriptor;
}
} // namespace Daemon
