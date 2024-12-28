#include "libClientManager.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

void attemptServerPortNameLookup(char*, const char*, const ConnectAttemptData*);

inline const ClientManager* Client_Init() {
  ClientManager* manager = malloc(sizeof(ClientManager));
  MPI_Comm* managerComm = &manager->comm;

  MPI_Comm_dup(MPI_COMM_WORLD, managerComm);
  MPI_Comm_set_errhandler(*managerComm, MPI_ERRORS_RETURN);

  return manager;
}

void Client_Finalize(const ClientManager* manager) {
  Client_Disconnect_Servers(manager);
  free(manager->servers.connections);

  MPI_Comm* managerComm = (MPI_Comm*)&manager->comm;
  if(managerComm != NULL) MPI_Comm_disconnect(managerComm);

  free((ClientManager*)manager);
}

void Client_Disconnect_Servers(const ClientManager* manager) {
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

const MPI_Comm* Client_Retrieve_Server_Comm(
  const ClientManager* manager, ConnectionId id, IdType idType
) {
  ConnectionsData* servers = (ConnectionsData*)&manager->servers;
  return &findConnectionOrError(servers, id, idType)->comm;
}

void attemptServerPortNameLookup(
  char* portName, const char* serverName, const ConnectAttemptData* data
) {
  const static ConnectAttemptData DEFAULT_ATTEMPT_DATA = {10, 1, 10};
  const ConnectAttemptData* parsedData =
    data == NULL ? &DEFAULT_ATTEMPT_DATA : data;

  const uint maxAttempts = parsedData->maxAttempts;
  const uint initialWait = parsedData->initialWaitTimeInSecs;
  const uint maxWait = parsedData->maxWaitTimeInSecs;

  uint attempts = 0;
  for(uint attempts = 0; attempts < maxAttempts; attempts++) {
    int errorValue = MPI_Lookup_name(serverName, MPI_INFO_NULL, portName);
    if(errorValue == MPI_SUCCESS) return;

    const uint waitTime = min(maxWait, initialWait + attempts);
    sleep(waitTime);
  }

  exitWithError(
    "Failed to lookup port name for server '", serverName, "' after ",
    maxAttempts, " attempts!"
  );
}