#include "mpmdManager.h"

typedef struct {
  uint leader, size;
} AuxiliarProgramData;

void setProgramsData(MPMDManager*, const char*, uint);
AuxiliarProgramData* getProgramsAuxiliarData(const char*, uint, uint*);
void setLocalData(const char*, const char*);

const MPMDManager* Manager_Init(char** argv) {
  MPMDManager* manager = malloc(sizeof(MPMDManager));
  MPI_Comm_dup(MPI_COMM_WORLD, &manager->managerComm);

  int worldSize, nameSize;
  MPI_Comm_size(manager->managerComm, &worldSize);

  char* gatheredNames = malloc(worldSize * NAME_MAX_SIZE * sizeof(char));
  getFilename(gatheredNames, &nameSize, argv);
  MPI_Allgather(
    gatheredNames, nameSize, MPI_CHAR, gatheredNames, NAME_MAX_SIZE, MPI_CHAR,
    manager->managerComm
  );
  setProgramsData(manager, gatheredNames, worldSize);

  free(gatheredNames);
  return manager;
}

void Manager_Finalize(MPMDManager* manager) { free(manager); }

char* Manager_Name(MPMDManager* manager) {}

int Manager_Size(MPMDManager* manager) {}

MPI_Comm* Manager_Intercomm_to(
  MPMDManager* manager, ProgramIdentifier identifier,
  IdentifierType identifierType
) {}

MPI_Comm* Manager_Size_of(
  MPMDManager* manager, ProgramIdentifier identifier,
  IdentifierType identifierType
) {}

void setProgramsData(
  MPMDManager* manager, const char* gatheredNames, uint worldSize
) {
  int worldRank;
  MPI_Comm_rank(manager->managerComm, &worldRank);

  uint programsSize;
  AuxiliarProgramData* auxiliarData =
    getProgramsAuxiliarData(gatheredNames, worldSize, &programsSize);

  free(auxiliarData);
}

AuxiliarProgramData* getProgramsAuxiliarData(
  const char* gatheredNames, uint worldSize, uint* programsSize
) {
  *programsSize = -1;
  AuxiliarProgramData* auxiliarData =
    malloc(worldSize * sizeof(AuxiliarProgramData));

  const char* previousName = NULL;
  const char* currentName = gatheredNames;

  for(int ind = 0; ind < worldSize; ind++) {
    if(streql(previousName, currentName)) auxiliarData[*programsSize].size++;
    else auxiliarData[++(*programsSize)] = (AuxiliarProgramData){ind, 1};

    previousName = currentName;
    currentName += NAME_MAX_SIZE;
  }

  ++(*programsSize);
  return auxiliarData;
}

void setLocalData(const char* localName, const char* gatheredNames) {}