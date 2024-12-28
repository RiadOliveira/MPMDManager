#include "managerAuxiliar.h"

bool streql(const char*, const char*);

void setConnections(MPMDManager* manager, char** argv) {
  AuxiliarData auxiliar;
  int worldRank, worldSize;
  MPI_Comm* managerComm = &manager->comm;

  MPI_Comm_rank(*managerComm, &worldRank);
  MPI_Comm_size(*managerComm, &worldSize);

  const char* gatheredNames = gatherNames(managerComm, argv, worldSize);
  getAuxiliarData(&auxiliar, gatheredNames, worldRank, worldSize);

  manager->connections = malloc(sizeof(Connection) * auxiliar.size);
  manager->connectionsSize = auxiliar.size;

  setLocalConnection(manager, &auxiliar, worldRank);
  setRemoteConnections(manager, &auxiliar);

  free(auxiliar.connections);
  free((char*)gatheredNames);
}

const char* gatherNames(MPI_Comm* managerComm, char** argv, uint worldSize) {
  uint nameSize;
  char* gatheredNames = malloc(sizeof(char) * worldSize * NAME_MAX_SIZE);

  getFilename(gatheredNames, &nameSize, argv);
  MPI_Allgather(
    gatheredNames, nameSize, MPI_CHAR, gatheredNames, NAME_MAX_SIZE, MPI_CHAR,
    *managerComm
  );

  return gatheredNames;
}

void getAuxiliarData(
  AuxiliarData* auxiliar, const char* gatheredNames, uint worldRank,
  uint worldSize
) {
  AuxiliarConnection* data = malloc(sizeof(AuxiliarConnection) * worldSize);
  auxiliar->connections = data;

  int connectionInd = -1;
  const char* previousName = NULL;
  const char* currentName = gatheredNames;

  for(int ind = 0; ind < worldSize; ind++) {
    if(streql(previousName, currentName)) data[connectionInd].size++;
    else {
      data[++connectionInd] = (AuxiliarConnection){currentName, ind, 1};
      if(ind <= worldRank) auxiliar->localInd = connectionInd;
    }

    previousName = currentName;
    currentName += NAME_MAX_SIZE;
  }
  auxiliar->size = connectionInd + 1;
}

void setLocalConnection(
  MPMDManager* manager, AuxiliarData* auxiliar, uint worldRank
) {
  MPI_Comm* managerComm = &manager->comm;
  uint localInd = manager->localInd = auxiliar->localInd;

  Connection* localConnection = &manager->connections[localInd];
  AuxiliarConnection* auxiliarLocal = &auxiliar->connections[localInd];

  strcpy(localConnection->name, auxiliarLocal->name);
  localConnection->size = auxiliarLocal->size;
  MPI_Comm_split(*managerComm, localInd, worldRank, &localConnection->comm);
}

void setRemoteConnections(MPMDManager* manager, AuxiliarData* auxiliar) {
  const uint connectionsSize = auxiliar->size;
  const uint localInd = auxiliar->localInd;
  MPI_Comm* localComm = &manager->connections[localInd].comm;

  for(uint ind = 0; ind < connectionsSize; ind++) {
    if(ind == localInd) continue;

    Connection* current = &manager->connections[ind];
    AuxiliarConnection* currentAuxiliar = &auxiliar->connections[ind];

    strcpy(current->name, currentAuxiliar->name);
    current->size = currentAuxiliar->size;

    MPI_Intercomm_create(
      *localComm, 0, manager->comm, currentAuxiliar->leader, 0, &current->comm
    );
  }
}

inline Connection* localConnection(MPMDManager* manager) {
  return &manager->connections[manager->localInd];
}

Connection* findConnectionOrError(
  MPMDManager* manager, ConnectionId id, IdType idType
) {
  Connection* connectionFound;
  const bool indexId = idType == INDEX_ID;

  if(indexId) connectionFound = findConnectionByIndex(manager, id.index);
  else connectionFound = findConnectionByName(manager, id.name);

  if(connectionFound == NULL) exitWithError("Connection not found!");
  return connectionFound;
}

inline Connection* findConnectionByIndex(MPMDManager* manager, uint index) {
  if(index >= manager->connectionsSize) return NULL;
  return &manager->connections[index];
}

Connection* findConnectionByName(MPMDManager* manager, const char* name) {
  if(name == NULL) return NULL;

  Connection* connections = manager->connections;
  const uint size = manager->connectionsSize;

  for(uint ind = 0; ind < size; ind++) {
    Connection* current = &connections[ind];
    if(streql(name, current->name)) return current;
  }

  return NULL;
}

inline bool streql(const char* first, const char* second) {
  if(first == NULL || second == NULL) return false;
  return strcmp(first, second) == 0;
}