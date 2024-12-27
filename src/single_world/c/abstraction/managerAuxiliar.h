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

void setProgramsData(MPMDManager* manager, char** argv);

const char* gatherNames(MPI_Comm* managerComm, char** argv, uint worldSize);
void getAuxiliarData(
  AuxiliarData* auxiliar, const char* gatheredNames, uint worldRank,
  uint worldSize
);

void setLocalProgramData(
  MPMDManager* manager, AuxiliarData* auxiliar, uint worldRank
);
void setRemoteProgramsData(MPMDManager* manager, AuxiliarData* auxiliar);

ProgramData* localProgram(const MPMDManager* manager);

ProgramData* findProgramOrError(
  const MPMDManager* manager, ProgramIdentifier identifier,
  IdentifierType identifierType
);
ProgramData* findProgramByIndex(const MPMDManager* manager, uint index);
ProgramData* findProgramByName(const MPMDManager* manager, const char* name);

bool streql(const char* first, const char* second);
