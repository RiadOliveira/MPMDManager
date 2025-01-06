#ifndef LIB_CLIENT_MANAGER
#define LIB_CLIENT_MANAGER

#include <math.h>
#include <unistd.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <time.h>
#endif

#include "connectionsData.h"

typedef struct {
  uint maxAttempts;
  uint initialWaitMs, waitIncrementMs, maxWaitMs;
} ConnectAttemptData;

typedef struct {
  ConnectionsData servers;
  MPI_Comm comm;
} ClientManager;

const ClientManager* Client_Init(uint maxServers);
void Client_Finalize(const ClientManager* manager);
void Client_Disconnect_servers(const ClientManager* manager);

const MPI_Comm* Client_Connect(
  const ClientManager* manager, const char* serverName,
  const ConnectAttemptData* attemptData
);
const MPI_Comm* Client_Retrieve_Server_comm(
  const ClientManager* manager, ConnectionId id, IdType idType
);

#endif