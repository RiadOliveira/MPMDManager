#ifndef LIB_MPMD_MANAGER
#define LIB_MPMD_MANAGER

#include <stdlib.h>

#include "programData.h"

typedef struct {
  ProgramData* programsData;
  uint programsQuantity, localProgramInd;
  MPI_Comm comm;
} MPMDManager;

const MPMDManager* Manager_Init(char** argv);
void Manager_Finalize(const MPMDManager* manager);

const char* Manager_Local_Name(const MPMDManager* manager);
const MPI_Comm* Manager_Local_Comm(const MPMDManager* manager);
int Manager_Local_Rank(const MPMDManager* manager);
uint Manager_Local_Size(const MPMDManager* manager);

const MPI_Comm* Manager_Intercomm_to(
  const MPMDManager* manager, ProgramIdentifier identifier,
  IdentifierType identifierType
);
uint Manager_Size_of(
  const MPMDManager*, ProgramIdentifier identifier,
  IdentifierType identifierType
);

#endif
