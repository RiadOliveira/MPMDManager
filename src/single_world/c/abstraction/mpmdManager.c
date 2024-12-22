#include "mpmdManager.h"

#include <stdio.h>

const MPMDManager* Manager_Init(char** argv) {
  MPMDManager* manager = malloc(sizeof(MPMDManager));
  MPI_Comm_dup(MPI_COMM_WORLD, &manager->managerComm);

  int worldSize, nameSize;
  MPI_Comm_size(manager->managerComm, &worldSize);

  char* gatheredNames = malloc(worldSize * FILENAME_MAX_SIZE * sizeof(char));
  getFilename(gatheredNames, &nameSize, argv);
  MPI_Allgather(
    gatheredNames, nameSize, MPI_CHAR, gatheredNames, FILENAME_MAX_SIZE,
    MPI_CHAR, manager->managerComm
  );

  free(gatheredNames);
  return manager;
}

void Manager_Finalize(MPMDManager*) {}

char* Manager_Name(MPMDManager*) {}

int Manager_Size(MPMDManager*) {}

MPI_Comm* Manager_Intercomm_to(
  MPMDManager*, ProgramIdentifier, IdentifierType
) {}

MPI_Comm* Manager_Size_of(MPMDManager*, ProgramIdentifier, IdentifierType) {}