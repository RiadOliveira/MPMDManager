#include "message.h"

void getMessageType(MPI_Datatype* MessageType) {
  Message auxiliar;
  MPI_Aint baseAddress, displacements[2];
  int lengths[2] = {20, 1};

  MPI_Get_address(&auxiliar, &baseAddress);
  MPI_Get_address(auxiliar.text, &displacements[0]);
  MPI_Get_address(&auxiliar.value, &displacements[1]);

  for(int ind = 0; ind < 2; ind++) {
    displacements[ind] = MPI_Aint_diff(displacements[ind], baseAddress);
  }

  MPI_Datatype types[2] = {MPI_CHAR, MPI_INT};
  MPI_Type_create_struct(2, lengths, displacements, types, MessageType);
  MPI_Type_commit(MessageType);
}

void printMessage(const char* prefix, Message* message) {
  printf("%s {text: '%s', value: %d}\n", prefix, message->text, message->value);
}