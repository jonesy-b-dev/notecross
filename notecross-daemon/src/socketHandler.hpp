#pragma once

namespace Daemon
{
int OpenSocket();
void HandleConnections(int socketFileDiscriptor);
} // namespace Daemon
