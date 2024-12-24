#include <mpi.h>
#include <stdio.h>

typedef struct {
  char text[30];
  int value;
} Message;

void getMessageType(MPI_Datatype* MessageType);

void printMessage(const char* prefix, Message* message);