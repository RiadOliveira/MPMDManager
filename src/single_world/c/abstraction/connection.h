#ifndef CONNECTION
#define CONNECTION

#include <mpi.h>

#include "filenameHandler.h"

typedef union {
  uint index;
  char* name;
} ConnectionId;

typedef enum { INDEX_ID, NAME_ID } IdType;

typedef struct {
  char name[NAME_MAX_SIZE];
  uint size;
  MPI_Comm comm;
} Connection;

#endif
