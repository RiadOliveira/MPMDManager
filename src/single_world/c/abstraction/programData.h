#include "mpi.h"

typedef struct {
  int idNum;
  int size;
  char* name;
  MPI_Comm* intercomm;
} ProgramData;