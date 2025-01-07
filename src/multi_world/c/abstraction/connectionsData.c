#include "connectionsData.h"

Connection* getNextUnactiveConnection(ConnectionsData*, uint*);
void setNewConnectionName(Connection*, const char*, uint);

Connection* findConnectionByIndex(ConnectionsData*, uint);
Connection* findConnectionByName(ConnectionsData*, const char*);

bool streql(const char*, const char*);

inline void initConnections(ConnectionsData* data, uint maxSize) {
  const uint parsedMax = maxSize == 0 ? DEFAULT_MAX_CONNECTIONS : maxSize;

  data->size = 0;
  data->maxSize = parsedMax;
  data->connections = malloc(sizeof(Connection) * parsedMax);

  Connection* connections = data->connections;
  for(uint ind = 0; ind < parsedMax; ind++) {
    Connection* current = &connections[ind];
    current->comm = MPI_COMM_NULL;
    current->active = false;
  }
}

inline void finalizeConnection(ConnectionsData* data, Connection* connection) {
  MPI_Comm* comm = &connection->comm;
  if(*comm != MPI_COMM_NULL) MPI_Comm_disconnect(comm);
  connection->active = false;

  data->size--;
}

inline void finalizeConnections(ConnectionsData* data) {
  const uint maxSize = data->maxSize;
  Connection* connections = data->connections;

  for(uint ind = 0; ind < maxSize; ind++) {
    finalizeConnection(data, &connections[ind]);
  }
}

inline void validateConnectionAddition(
  ConnectionsData* data, const char* name
) {
  const bool maxSizeReached = data->size >= data->maxSize;
  if(maxSizeReached) exitWithError("Connections max size reached!");

  const bool existsByName = findConnectionByName(data, name) != NULL;
  if(existsByName) {
    exitWithError("A connection named '%s' already exists!", name);
  }
}

Connection* addConnection(
  ConnectionsData* data, const char* name, MPI_Comm* comm
) {
  uint ind;
  Connection* newConnection = getNextUnactiveConnection(data, &ind);
  setNewConnectionName(newConnection, name, ind);
  newConnection->active = true;
  newConnection->comm = *comm;

  data->size++;
  return newConnection;
}

inline Connection* findConnectionOrError(
  ConnectionsData* data, ConnectionId id, IdType idType
) {
  Connection* connectionFound;
  const bool indexId = idType == INDEX_ID;

  if(indexId) connectionFound = findConnectionByIndex(data, id.index);
  else connectionFound = findConnectionByName(data, id.name);

  if(connectionFound == NULL) exitWithError("Connection not found!");
  return connectionFound;
}

Connection* getNextUnactiveConnection(ConnectionsData* data, uint* ind) {
  const uint size = data->size;
  Connection* connections = data->connections;

  Connection* current;
  if(!connections[size].active) current = &connections[size];
  else {
    *ind = 0;
    current = connections;

    while(current->active) {
      current++;
      (*ind)++;
    }
  }

  return current;
}

inline void setNewConnectionName(
  Connection* newConnection, const char* name, uint connectionInd
) {
  char* connectionName = newConnection->name;

  if(name != NULL) strcpy(connectionName, name);
  else sprintf(connectionName, "%d", connectionInd);
}

inline Connection* findConnectionByIndex(ConnectionsData* data, uint index) {
  if(index >= data->maxSize) return NULL;

  Connection* connectionFound = &data->connections[index];
  return connectionFound->active ? connectionFound : NULL;
}

inline Connection* findConnectionByName(
  ConnectionsData* data, const char* name
) {
  if(name == NULL) return NULL;

  Connection* connections = data->connections;
  const uint maxSize = data->maxSize;

  for(uint ind = 0; ind < maxSize; ind++) {
    Connection* current = &connections[ind];
    if(!current->active) continue;
    if(streql(name, current->name)) return current;
  }

  return NULL;
}

inline bool streql(const char* first, const char* second) {
  if(first == NULL || second == NULL) return false;
  return strcmp(first, second) == 0;
}
