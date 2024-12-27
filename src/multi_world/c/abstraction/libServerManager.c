#include "libServerManager.h"

void setServerName(ServerManager*, char**, const char*);

const ServerManager* Server_Init(
  char** argv, const char* serverName, uint maxClientsQuantity
) {
  ServerManager* manager = malloc(sizeof(ServerManager));
  MPI_Comm* managerComm = &manager->comm;

  MPI_Comm_dup(MPI_COMM_WORLD, managerComm);
  setServerName(manager, argv, serverName);

  manager->maxClientsQuantity = maxClientsQuantity;
  manager->clientsData = malloc(sizeof(ProgramData) * maxClientsQuantity);

  return manager;
}

void Server_Finalize(const ServerManager* manager) {}

void Server_Open(const ServerManager* manager) {}

void Server_Close(const ServerManager* manager) {}

void Server_Accept(const ServerManager* manager) {}

void Server_Comm_to_Connected_Client(
  ProgramIdentifier identifier, IdentifierType IdentifierType
) {}

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