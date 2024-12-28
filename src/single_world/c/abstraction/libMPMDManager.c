#include "managerAuxiliar.h"

const MPMDManager* Manager_Init(char** argv) {
  MPMDManager* manager = malloc(sizeof(MPMDManager));
  MPI_Comm* managerComm = &manager->comm;
  MPI_Comm_dup(MPI_COMM_WORLD, managerComm);

  setConnections(manager, argv);
  return manager;
}

void Manager_Finalize(const MPMDManager* manager) {
  Connection* connections = manager->connections;
  const uint connectionsSize = manager->connectionsSize;

  for(uint ind = 0; ind < connectionsSize; ind++) {
    MPI_Comm* connectionComm = &connections[ind].comm;
    if(connectionComm != NULL) MPI_Comm_disconnect(connectionComm);
  }
  free(manager->connections);

  MPI_Comm* managerComm = (MPI_Comm*)&manager->comm;
  if(managerComm != NULL) MPI_Comm_disconnect(managerComm);

  free((MPMDManager*)manager);
}

inline const char* Manager_Local_Name(const MPMDManager* manager) {
  return localConnection(manager)->name;
}

inline const MPI_Comm* Manager_Local_Comm(const MPMDManager* manager) {
  return &localConnection(manager)->comm;
}

inline int Manager_Local_Rank(const MPMDManager* manager) {
  const MPI_Comm* localComm = Manager_Local_Comm(manager);
  int rank;

  MPI_Comm_rank(*localComm, &rank);
  return rank;
}

inline uint Manager_Local_Size(const MPMDManager* manager) {
  return localConnection(manager)->size;
}

inline const MPI_Comm* Manager_Comm_to(
  const MPMDManager* manager, ConnectionId id, IdType idType
) {
  return &findConnectionOrError(manager, id, idType)->comm;
}

inline uint Manager_Size_of(
  const MPMDManager* manager, ConnectionId id, IdType idType
) {
  return findConnectionOrError(manager, id, idType)->size;
}
