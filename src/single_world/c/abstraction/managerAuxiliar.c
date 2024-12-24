#include "managerAuxiliar.h"

void setProgramsData(MPMDManager* manager, char** argv) {
  AuxiliarData auxiliar;
  int worldRank, worldSize;
  MPI_Comm* managerComm = &manager->comm;

  MPI_Comm_rank(*managerComm, &worldRank);
  MPI_Comm_size(*managerComm, &worldSize);

  const char* gatheredNames = gatherNames(managerComm, argv, worldSize);
  getAuxiliarData(&auxiliar, gatheredNames, worldSize, worldRank);

  manager->programsData = malloc(sizeof(ProgramData) * auxiliar.quantity);
  manager->programsQuantity = auxiliar.quantity;

  setLocalProgramData(manager, &auxiliar, worldRank);
  setRemoteProgramsData(manager, &auxiliar);

  free(auxiliar.programs);
  free((char*)gatheredNames);
}

const char* gatherNames(MPI_Comm* managerComm, char** argv, uint worldSize) {
  int nameSize;
  char* gatheredNames = malloc(sizeof(char) * worldSize * NAME_MAX_SIZE);

  getFilename(gatheredNames, &nameSize, argv);
  MPI_Allgather(
    gatheredNames, nameSize, MPI_CHAR, gatheredNames, NAME_MAX_SIZE, MPI_CHAR,
    *managerComm
  );

  return gatheredNames;
}

void getAuxiliarData(
  AuxiliarData* auxiliarData, const char* gatheredNames, uint worldSize,
  uint worldRank
) {
  AuxiliarProgramData* data = malloc(sizeof(AuxiliarProgramData) * worldSize);
  auxiliarData->programs = data;

  int programInd = -1;
  const char* previousName = NULL;
  const char* currentName = gatheredNames;

  for(int ind = 0; ind < worldSize; ind++) {
    if(streql(previousName, currentName)) data[programInd].size++;
    else {
      data[++programInd] = (AuxiliarProgramData){currentName, ind, 1};
      if(ind <= worldRank) auxiliarData->localProgramInd = programInd;
    }

    previousName = currentName;
    currentName += NAME_MAX_SIZE;
  }
  auxiliarData->quantity = programInd + 1;
}

void setLocalProgramData(
  MPMDManager* manager, AuxiliarData* auxiliar, uint worldRank
) {
  MPI_Comm* managerComm = &manager->comm;
  uint localProgramInd = manager->localProgramInd = auxiliar->localProgramInd;

  ProgramData* localProgram = &manager->programsData[localProgramInd];
  AuxiliarProgramData* auxiliarLocal = &auxiliar->programs[localProgramInd];

  strcpy(localProgram->name, auxiliarLocal->name);
  localProgram->size = auxiliarLocal->size;
  MPI_Comm_split(*managerComm, localProgramInd, worldRank, &localProgram->comm);
}

void setRemoteProgramsData(MPMDManager* manager, AuxiliarData* auxiliar) {
  const uint programsQuantity = auxiliar->quantity;
  const uint localProgramInd = auxiliar->localProgramInd;
  ProgramData* localProgram = &manager->programsData[localProgramInd];

  for(uint ind = 0; ind < programsQuantity; ind++) {
    if(ind == localProgramInd) continue;

    ProgramData* currentProgram = &manager->programsData[ind];
    AuxiliarProgramData* currentAuxiliar = &auxiliar->programs[ind];

    strcpy(currentProgram->name, currentAuxiliar->name);
    currentProgram->size = currentAuxiliar->size;

    MPI_Intercomm_create(
      localProgram->comm, 0, manager->comm, currentAuxiliar->leader, 0,
      &currentProgram->comm
    );
  }
}

ProgramData* localProgram(const MPMDManager* manager) {
  return &manager->programsData[manager->localProgramInd];
}

ProgramData* findProgramOrError(
  const MPMDManager* manager, ProgramIdentifier identifier,
  IdentifierType identifierType
) {
  ProgramData* programFound;
  const bool indexId = identifierType == INDEX_ID;

  if(indexId) programFound = findProgramByIndex(manager, identifier.index);
  else programFound = findProgramByName(manager, identifier.name);

  if(programFound == NULL) MPI_Abort(manager->comm, EXIT_FAILURE);
  return programFound;
}

ProgramData* findProgramByIndex(const MPMDManager* manager, uint index) {
  if(index >= manager->programsQuantity) return NULL;
  return &manager->programsData[index];
}

ProgramData* findProgramByName(const MPMDManager* manager, const char* name) {
  ProgramData* programsData = manager->programsData;
  const uint programsQuantity = manager->programsQuantity;

  for(uint ind = 0; ind < programsQuantity; ind++) {
    ProgramData* currentProgram = &programsData[ind];
    if(streql(name, currentProgram->name)) return currentProgram;
  }

  return NULL;
}

bool streql(const char* first, const char* second) {
  if(first == NULL || second == NULL) return false;
  return strcmp(first, second) == 0;
}