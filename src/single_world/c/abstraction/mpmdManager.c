#include "mpmdManager.h"

MPMDManager Manager_Initialize() {}

void Manager_Finalize(MPMDManager*) {}

char* Manager_Name(MPMDManager*) {}

int Manager_Size(MPMDManager*) {}

MPI_Comm* Manager_Intercomm_to(
  MPMDManager*, ProgramIdentifier, IdentifierType
) {}

MPI_Comm* Manager_Size_of(MPMDManager*, ProgramIdentifier, IdentifierType) {}