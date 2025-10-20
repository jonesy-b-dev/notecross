#pragma once
#define SERVER_SOCK_PATH "/tmp/NoteCrossDaemonSocket"

namespace Daemon
{
int OpenSocket();
void HandleConnections(int socketFileDiscriptor);
} // namespace Daemon
