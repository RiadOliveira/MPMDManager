#ifndef LIB_SERVER_MANAGER
#define LIB_SERVER_MANAGER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connectionsData.h"
#include "filenameHandler.h"

typedef struct {
  char name[NAME_MAX_SIZE], portName[MPI_MAX_PORT_NAME];
  ConnectionsData clientsData;
  MPI_Comm comm;
} ServerManager;

const ServerManager* Server_Init(
  char** argv, const char* serverName, uint maxClients
);
void Server_Finalize(const ServerManager* manager);
void Server_Disconnect_Clients(const ServerManager* manager);

void Server_Open(const ServerManager* manager);
void Server_Close(const ServerManager* manager);

void Server_Accept(const ServerManager* manager);
void Server_Comm_to_Connected_Client(
  ProgramIdentifier identifier, IdentifierType IdentifierType
);

const char* Server_Name(const ServerManager* manager);

#endif