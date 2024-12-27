#ifndef CONNECTIONS_DATA
#define CONNECTIONS_DATA

#define DEFAULT_MAX_CONNECTIONS 5

#include "connection.h"

typedef struct {
  Connection* connections;
  uint size, maxSize;
} ConnectionsData;

void initConnections(ConnectionsData* data, uint maxSize);
void finalizeConnections(ConnectionsData* data);

#endif