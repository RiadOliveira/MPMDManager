#include <mpi.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char text[30];
  int value;
} Message;

void getMessageType(MPI_Datatype* MessageType);

void fillMessage(Message* message, char* text, int value);

void printMessage(const char* prefix, Message* message);