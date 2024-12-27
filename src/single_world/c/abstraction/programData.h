#include <mpi.h>

#include "filenameHandler.h"

typedef union {
  uint index;
  char* name;
} ProgramIdentifier;

typedef enum { INDEX_ID, NAME_ID } IdentifierType;

typedef struct {
  char name[NAME_MAX_SIZE];
  uint size;
  MPI_Comm comm;
} ProgramData;
