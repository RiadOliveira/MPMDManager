#include "libServerManager.h"

void setServerName(ServerManager*, char**, const char*);

const ServerManager* Server_Init(
  char** argv, const char* serverName, uint maxClients
) {
  ServerManager* manager = malloc(sizeof(ServerManager));

  MPI_Comm_dup(MPI_COMM_WORLD, &manager->comm);
  setServerName(manager, argv, serverName);
  initConnections(&manager->clientsData, maxClients);

  return manager;
}

void Server_Finalize(const ServerManager* manager) {
  ConnectionsData* clientsData = &manager->clientsData;
  finalizeConnections(clientsData);
  free(clientsData->connections);

  MPI_Comm* managerComm = (MPI_Comm*)&manager->comm;
  if(managerComm != NULL) MPI_Comm_disconnect(managerComm);

  Server_Close(manager);
  free((ServerManager*)manager);
}

void Server_Disconnect_Clients(const ServerManager* manager) {}

void Server_Open(const ServerManager* manager) {
  char* portName = (char*)manager->portName;

  MPI_Open_port(MPI_INFO_NULL, portName);
  MPI_Publish_name(manager->name, MPI_INFO_NULL, portName);
}

void Server_Close(const ServerManager* manager) {
  const char* portName = manager->portName;

  MPI_Unpublish_name(manager->name, MPI_INFO_NULL, portName);
  MPI_Close_port(portName);
}

void Server_Accept(
  const ServerManager* manager, ConnectionId id, IdType idType
) {}

void Server_Comm_to_Connected_Client(ConnectionId id, IdType idType) {}

const char* Server_Name(const ServerManager* manager) { return manager->name; }

void setServerName(
  ServerManager* manager, char** argv, const char* serverName
) {
  char* managerName = manager->name;

  if(serverName != NULL) {
    strcpy(managerName, serverName);
    return;
  }

  uint rank, nameSize;
  MPI_Comm_rank(manager->comm, &rank);

  getFilename(managerName, &nameSize, argv);
  sprintf(&managerName[nameSize], "%d", rank);
}