#define SOCK_PATH "/tmp/NoteCrossDaemonSocket"
#include "socketHandler.hpp"
#include <iostream>
#include "socketHandler.hpp"
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace NCCLI
{
int OpenSocket()
{
    struct sockaddr_un clientSockAddr{};

    int socketFileDiscriptor = socket(AF_UNIX, SOCK_STREAM, 0);

    if (socketFileDiscriptor == -1)
    {
        std::cerr << "Failed to open Notecross client socket!\n";
        close(socketFileDiscriptor);
        exit(1);
    }
    clientSockAddr.sun_family = AF_UNIX;
    strncpy(clientSockAddr.sun_path, SOCK_PATH, sizeof(clientSockAddr.sun_path) - 1);
    if (connect(socketFileDiscriptor,
                (struct sockaddr*)&clientSockAddr,
                sizeof(struct sockaddr_un)) == -1)
    {
        std::cerr << "Error connecting to the daemon socket!\n";
        exit(1);
    }
    // std::cout << "Connected to the daemon socket! \n";
    return socketFileDiscriptor;
}
} // namespace NCCLI
