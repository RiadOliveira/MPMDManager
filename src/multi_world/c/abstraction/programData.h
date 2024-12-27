#ifndef PROGRAM_DATA
#define PROGRAM_DATA

#include <mpi.h>

#include "shortcutTypes.h"

typedef union {
  uint index;
  char* name;
} ProgramIdentifier;

typedef enum { INDEX_ID, NAME_ID } IdentifierType;

typedef struct {
  bool active;
  ProgramIdentifier identifier;
  IdentifierType identifierType;
  MPI_Comm comm;
} ProgramData;

#endif