#ifndef MPMD_MANAGER
#define MPMD_MANAGER

#include "programData.h"

typedef struct {
  char* localName;
  ProgramData* programsData;
  MPI_Comm* localComm;
  MPI_Comm* managerComm;
} MPMDManager;

typedef union {
  char* name;
  int idNum;
} ProgramIdentifier;

typedef enum { NAME, ID_NUM } IdentifierType;

MPMDManager Manager_Initialize();
void Manager_Finalize(MPMDManager*);

char* Manager_Name(MPMDManager*);
int Manager_Size(MPMDManager*);

MPI_Comm* Manager_Intercomm_to(MPMDManager*, ProgramIdentifier, IdentifierType);
MPI_Comm* Manager_Size_of(MPMDManager*, ProgramIdentifier, IdentifierType);

#endif
