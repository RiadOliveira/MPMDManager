#ifndef CONNECTIONS_DATA
#define CONNECTIONS_DATA

#define DEFAULT_MAX_CONNECTIONS 5

#include "programData.h"

typedef struct {
  ProgramData* connections;
  uint size, maxSize;
} ConnectionsData;

void initConnections(ConnectionsData* data, uint maxSize);
void finalizeConnections(ConnectionsData* data);

#endif