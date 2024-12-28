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
  Server_Disconnect_Clients(manager);
  free(manager->clientsData.connections);

  MPI_Comm* managerComm = (MPI_Comm*)&manager->comm;
  if(managerComm != NULL) MPI_Comm_disconnect(managerComm);

  Server_Close(manager);
  free((ServerManager*)manager);
}

inline void Server_Disconnect_Clients(const ServerManager* manager) {
  ConnectionsData* clientsData = (ConnectionsData*)&manager->clientsData;
  finalizeConnections(clientsData);
}

inline void Server_Open(const ServerManager* manager) {
  char* portName = (char*)manager->portName;

  MPI_Open_port(MPI_INFO_NULL, portName);
  MPI_Publish_name(manager->name, MPI_INFO_NULL, portName);
}

inline void Server_Close(const ServerManager* manager) {
  const char* portName = manager->portName;

  MPI_Unpublish_name(manager->name, MPI_INFO_NULL, portName);
  MPI_Close_port(portName);
}

const MPI_Comm* Server_Accept(
  const ServerManager* manager, const char* clientName
) {
  MPI_Comm clientComm;
  char* portName = (char*)manager->portName;
  MPI_Comm_accept(portName, MPI_INFO_NULL, 0, manager->comm, &clientComm);

  ConnectionsData* clientsData = (ConnectionsData*)&manager->clientsData;
  Connection* client = addConnection(clientsData, clientName, &clientComm);
  return &client->comm;
}

const MPI_Comm* Server_Retrieve_Client_Comm(ConnectionId id, IdType idType) {}

inline const char* Server_Name(const ServerManager* manager) {
  return manager->name;
}

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