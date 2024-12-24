#include <string.h>

#include "libMPMDManager.h"

typedef struct {
  const char* name;
  uint leader, size;
} AuxiliarProgramData;

typedef struct {
  AuxiliarProgramData* programs;
  uint localProgramInd, quantity;
} AuxiliarData;

void setProgramsData(MPMDManager*, char**);

const char* gatherNames(MPI_Comm*, char**, uint);
void getAuxiliarData(AuxiliarData*, const char*, uint, uint);

void setLocalProgramData(MPMDManager*, AuxiliarData*, uint);
void setRemoteProgramsData(MPMDManager*, AuxiliarData*);

ProgramData* localProgram(const MPMDManager*);

ProgramData* findProgramOrError(
  const MPMDManager*, ProgramIdentifier, IdentifierType
);
ProgramData* findProgramByIndex(const MPMDManager*, uint);
ProgramData* findProgramByName(const MPMDManager*, const char*);

bool streql(const char* first, const char* second);
