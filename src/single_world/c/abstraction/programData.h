#include <mpi.h>

#include "filenameHandler.h"

typedef struct {
  char name[FILENAME_MAX_SIZE];
  uint size;
  MPI_Comm* comm;
} ProgramData;