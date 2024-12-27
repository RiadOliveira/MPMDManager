#ifndef LIB_MPMD_MANAGER
#define LIB_MPMD_MANAGER

#include <stdlib.h>

#include "connection.h"

typedef struct {
  Connection* connections;
  uint connectionsSize, localInd;
  MPI_Comm comm;
} MPMDManager;

const MPMDManager* Manager_Init(char** argv);
void Manager_Finalize(const MPMDManager* manager);

const char* Manager_Local_Name(const MPMDManager* manager);
const MPI_Comm* Manager_Local_Comm(const MPMDManager* manager);
int Manager_Local_Rank(const MPMDManager* manager);
uint Manager_Local_Size(const MPMDManager* manager);

const MPI_Comm* Manager_Comm_to(
  const MPMDManager* manager, ConnectionId id, IdType idType
);
uint Manager_Size_of(const MPMDManager*, ConnectionId id, IdType idType);

#endif
