#ifndef MANAGER_AUXILIAR
#define MANAGER_AUXILIAR

#include <string.h>

#include "error.h"
#include "libMPMDManager.h"

typedef struct {
  const char* name;
  uint leader, size;
} AuxiliarConnection;

typedef struct {
  AuxiliarConnection* connections;
  uint localInd, size;
} AuxiliarData;

void setConnections(MPMDManager* manager, char** argv);

const char* gatherNames(MPI_Comm* managerComm, char** argv, uint worldSize);
void getAuxiliarData(
  AuxiliarData* auxiliar, const char* gatheredNames, uint worldRank,
  uint worldSize
);

void setLocalConnection(
  MPMDManager* manager, AuxiliarData* auxiliar, uint worldRank
);
void setRemoteConnections(MPMDManager* manager, AuxiliarData* auxiliar);

Connection* localConnection(MPMDManager* manager);

Connection* findConnectionOrError(
  MPMDManager* manager, ConnectionId id, IdType idType
);
Connection* findConnectionByIndex(MPMDManager* manager, uint index);
Connection* findConnectionByName(MPMDManager* manager, const char* name);

#endif