#include "connectionsData.h"

void initConnections(ConnectionsData* data, uint maxSize) {
  const uint parsedMax = maxSize == 0 ? DEFAULT_MAX_CONNECTIONS : maxSize;

  data->size = 0;
  data->maxSize = parsedMax;
  data->connections = malloc(sizeof(ProgramData) * parsedMax);

  ProgramData* connections = data->connections;
  for(uint ind = 0; ind < parsedMax; ind++) {
    connections[ind].active = false;
  }
}

void finalizeConnections(ConnectionsData* data) {
  ProgramData* connections = data->connections;
  const uint maxSize = data->maxSize;

  for(uint ind = 0; ind < maxSize; ind++) {
    MPI_Comm* programComm = &connections[ind].comm;
    if(programComm != NULL) MPI_Comm_disconnect(programComm);
  }
}

void addConnection(ConnectionsData* data, ProgramData* connection) {}