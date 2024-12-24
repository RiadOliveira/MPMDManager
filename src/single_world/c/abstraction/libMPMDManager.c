#include "managerAuxiliar.h"

const MPMDManager* Manager_Init(char** argv) {
  MPMDManager* manager = malloc(sizeof(MPMDManager));
  MPI_Comm* managerComm = &manager->comm;
  MPI_Comm_dup(MPI_COMM_WORLD, managerComm);

  setProgramsData(manager, argv);
  return manager;
}

void Manager_Finalize(const MPMDManager* manager) {
  ProgramData* programs = manager->programsData;
  const uint programsQuantity = manager->programsQuantity;

  for(uint ind = 0; ind < programsQuantity; ind++) {
    MPI_Comm* programComm = &programs[ind].comm;
    if(programComm != NULL) MPI_Comm_disconnect(programComm);
  }
  free(manager->programsData);

  MPI_Comm* managerComm = (MPI_Comm*)&manager->comm;
  if(managerComm != NULL) MPI_Comm_disconnect(managerComm);

  free((MPMDManager*)manager);
}

const char* Manager_Local_Name(const MPMDManager* manager) {
  return localProgram(manager)->name;
}

const MPI_Comm* Manager_Local_Comm(const MPMDManager* manager) {
  return &localProgram(manager)->comm;
}

int Manager_Local_Rank(const MPMDManager* manager) {
  const MPI_Comm* localComm = Manager_Local_Comm(manager);
  int rank;

  MPI_Comm_rank(*localComm, &rank);
  return rank;
}

uint Manager_Local_Size(const MPMDManager* manager) {
  return localProgram(manager)->size;
}

const MPI_Comm* Manager_Intercomm_to(
  const MPMDManager* manager, ProgramIdentifier identifier,
  IdentifierType identifierType
) {
  return &findProgramOrError(manager, identifier, identifierType)->comm;
}

uint Manager_Size_of(
  const MPMDManager* manager, ProgramIdentifier identifier,
  IdentifierType identifierType
) {
  return findProgramOrError(manager, identifier, identifierType)->size;
}
