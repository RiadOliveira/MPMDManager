#include "libClientManager.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

void attemptServerPortNameLookup(char*, const char*, const ConnectAttemptData*);
uint getCurrentWaitTime(const ConnectAttemptData*, uint);
void sleepMs(uint);

inline const ClientManager* Client_Init(uint maxServers) {
  ClientManager* manager = malloc(sizeof(ClientManager));

  MPI_Comm_dup(MPI_COMM_SELF, &manager->comm);
  MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  initConnections(&manager->servers, maxServers);

  return manager;
}

inline void Client_Finalize(const ClientManager* manager) {
  Client_Disconnect_servers(manager);
  free(manager->servers.connections);

  MPI_Comm* managerComm = (MPI_Comm*)&manager->comm;
  if(managerComm != NULL) MPI_Comm_disconnect(managerComm);

  free((ClientManager*)manager);
}

inline void Client_Disconnect_servers(const ClientManager* manager) {
  ConnectionsData* servers = (ConnectionsData*)&manager->servers;
  finalizeConnections(servers);
}

const MPI_Comm* Client_Connect(
  const ClientManager* manager, const char* serverName,
  const ConnectAttemptData* attemptData
) {
  char portName[MPI_MAX_PORT_NAME];
  attemptServerPortNameLookup(portName, serverName, attemptData);

  MPI_Comm serverComm;
  MPI_Comm_connect(portName, MPI_INFO_NULL, 0, manager->comm, &serverComm);

  ConnectionsData* servers = (ConnectionsData*)&manager->servers;
  Connection* server = addConnection(servers, serverName, &serverComm);
  return &server->comm;
}

inline const MPI_Comm* Client_Retrieve_Server_comm(
  const ClientManager* manager, ConnectionId id, IdType idType
) {
  ConnectionsData* servers = (ConnectionsData*)&manager->servers;
  return &findConnectionOrError(servers, id, idType)->comm;
}

void attemptServerPortNameLookup(
  char* portName, const char* serverName, const ConnectAttemptData* data
) {
  const static ConnectAttemptData DEFAULT_DATA = {10U, 500U, 500U, 5000U};

  const ConnectAttemptData* parsedData = data == NULL ? &DEFAULT_DATA : data;
  const uint maxAttempts = parsedData->maxAttempts;

  MPI_Errhandler originalErrorHandler;
  MPI_Comm_get_errhandler(MPI_COMM_WORLD, &originalErrorHandler);
  MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  for(uint attempts = 0; attempts < maxAttempts; attempts++) {
    int errorValue = MPI_Lookup_name(serverName, MPI_INFO_NULL, portName);
    if(errorValue == MPI_SUCCESS) {
      MPI_Comm_set_errhandler(MPI_COMM_WORLD, originalErrorHandler);
      return;
    }

    sleepMs(getCurrentWaitTime(parsedData, attempts));
  }

  exitWithError(
    "Failed to lookup port name for server '", serverName, "' after ",
    maxAttempts, " attempts!"
  );
}

inline uint getCurrentWaitTime(const ConnectAttemptData* data, uint attempts) {
  return min(
    data->maxWaitMs, data->initialWaitMs + attempts * data->waitIncrementMs
  );
}

inline void sleepMs(uint milliseconds) {
#ifdef _WIN32
  Sleep(milliseconds);
#else
  struct timespec ts;

  ts.tv_sec = milliseconds / 1000U;
  const uint remainingMs = milliseconds - (ts.tv_sec * 1000U);
  ts.tv_nsec = remainingMs * 1000000L;

  nanosleep(&ts, NULL);
#endif
}