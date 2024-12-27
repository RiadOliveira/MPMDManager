#ifndef PROGRAM_DATA
#define PROGRAM_DATA

#include <mpi.h>

#include "shortcutTypes.h"

typedef union {
  uint index;
  const char* name;
} ProgramIdentifier;

typedef enum { INDEX_ID, NAME_ID } IdentifierType;

typedef struct {
  ProgramIdentifier identifier;
  IdentifierType IdentifierType;
  MPI_Comm comm;
} ProgramData;

#endif