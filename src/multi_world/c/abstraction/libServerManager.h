#ifndef LIB_SERVER_MANAGER
#define LIB_SERVER_MANAGER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connectionsData.h"
#include "filenameHandler.h"

typedef struct {
  char name[NAME_MAX_SIZE], portName[MPI_MAX_PORT_NAME];
  ConnectionsData clients;
  MPI_Comm comm;
} ServerManager;

const ServerManager* Server_Init(
  char** argv, const char* serverName, uint maxClients
);
void Server_Finalize(const ServerManager* manager);

const char* Server_Local_name(const ServerManager* manager);

void Server_Open(const ServerManager* manager);
void Server_Close(const ServerManager* manager);

const MPI_Comm* Server_Accept(
  const ServerManager* manager, const char* clientName
);
const MPI_Comm* Server_Retrieve_Client_comm(
  const ServerManager* manager, ConnectionId id, IdType idType
);

void Server_Disconnect_client(
  const ServerManager* manager, ConnectionId id, IdType idType
);
void Server_Disconnect_clients(const ServerManager* manager);

#endif