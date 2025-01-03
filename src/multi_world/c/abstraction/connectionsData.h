#ifndef CONNECTIONS_DATA
#define CONNECTIONS_DATA

#define DEFAULT_MAX_CONNECTIONS 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connection.h"
#include "error.h"

typedef struct {
  Connection* connections;
  uint size, maxSize;
} ConnectionsData;

void initConnections(ConnectionsData* data, uint maxSize);
void finalizeConnections(ConnectionsData* data);

Connection* addConnection(
  ConnectionsData* data, const char* name, MPI_Comm* comm
);
Connection* findConnectionOrError(
  ConnectionsData* data, ConnectionId id, IdType idType
);

#endif