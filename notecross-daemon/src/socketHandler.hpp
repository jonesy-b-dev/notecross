#pragma once

namespace NCDaemon
{
int OpenSocket();
void HandleConnections(int socketFileDiscriptor);
} // namespace NCDaemon
