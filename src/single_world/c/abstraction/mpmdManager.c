#include "mpmdManager.h"

typedef struct {
  uint leader, size;
} AuxiliarProgramData;

void setProgramsData(MPMDManager*, const char*, uint);
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
  MPMDManager* manager, const char* gatheredNames, uint namesQuantity
) {
  int worldRank;
  MPI_Comm_rank(manager->managerComm, &worldRank);

  AuxiliarProgramData* leaders =
    malloc(namesQuantity * sizeof(AuxiliarProgramData));

  const char* previousName = NULL;
  const char* currentName = gatheredNames;
  // const char* localName = &gatheredNames[worldRank * NAME_MAX_SIZE];

  uint ind = 0, programInd = 0, localInd;
  leaders[0] = (AuxiliarProgramData){0, 1};

  while(ind++ < namesQuantity) {
  }

  free(leaders);
}

void setLocalData(const char* localName, const char* gatheredNames) {}