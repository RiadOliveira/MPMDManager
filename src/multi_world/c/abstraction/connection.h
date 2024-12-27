#ifndef CONNECTION
#define CONNECTION

#include <mpi.h>

#include "shortcutTypes.h"

typedef union {
  uint index;
  char* name;
} ConnectionId;

typedef enum { INDEX_ID, NAME_ID } IdType;

typedef struct {
  bool active;
  ConnectionId id;
  IdType idType;
  MPI_Comm comm;
} Connection;

#endif