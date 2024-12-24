#include "message.h"
#include "mpmdManager.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  const MPMDManager* manager = Manager_Init(argv);

  int localRank;
  const MPI_Comm* localComm = Manager_Local_Comm(manager);
  MPI_Comm_rank(*localComm, &localRank);

  Message message;
  MPI_Datatype MessageType;
  getMessageType(&MessageType);

  char programName[10];
  ProgramIdentifier nameId = {.name = programName};

  if(localRank == 0) {
    strcpy(message.text, "0|Second to 1|Third");
    message.value = 2031;

    strcpy(programName, "third");
    const MPI_Comm* thirdIntercomm =
      Manager_Intercomm_to(manager, nameId, NAME_ID);

    MPI_Send(&message, 1, MessageType, 1, 0, *thirdIntercomm);
    printMessage("(Send) 0|Second -> 1|Third: ", &message);
  } else if(localRank == 1) {
    strcpy(programName, "first");
    const MPI_Comm* firstIntercomm =
      Manager_Intercomm_to(manager, nameId, NAME_ID);

    MPI_Recv(
      &message, 1, MessageType, 0, 0, *firstIntercomm, MPI_STATUS_IGNORE
    );
    printMessage("(Recv) 1|Second <- 0|First: ", &message);
  }

  Manager_Finalize(manager);
  MPI_Finalize();
}