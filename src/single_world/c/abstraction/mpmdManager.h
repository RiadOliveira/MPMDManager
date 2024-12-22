#ifndef MPMD_MANAGER
#define MPMD_MANAGER

#include <stdlib.h>

#include "programData.h"
#include "utils.h"

typedef struct {
  ProgramData* localData;
  ProgramData* programsData;
  MPI_Comm managerComm;
} MPMDManager;

typedef union {
  char* name;
  int index;
} ProgramIdentifier;

typedef enum { NAME, INDEX } IdentifierType;

const MPMDManager* Manager_Init(char** argv);
void Manager_Finalize(MPMDManager*);

char* Manager_Name(MPMDManager*);
int Manager_Size(MPMDManager*);

MPI_Comm* Manager_Intercomm_to(MPMDManager*, ProgramIdentifier, IdentifierType);
MPI_Comm* Manager_Size_of(MPMDManager*, ProgramIdentifier, IdentifierType);

#endif
