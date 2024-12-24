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
    strcpy(message.text, "0|Third  to 1|First");
    message.value = 3011;

    strcpy(programName, "first");
    const MPI_Comm* firstIntercomm =
      Manager_Intercomm_to(manager, nameId, NAME_ID);

    MPI_Send(&message, 1, MessageType, 1, 0, *firstIntercomm);
    printMessage("(Send) 0|Third  -> 1|First: ", &message);
  } else if(localRank == 1) {
    strcpy(programName, "second");
    const MPI_Comm* secondIntercomm =
      Manager_Intercomm_to(manager, nameId, NAME_ID);

    MPI_Recv(
      &message, 1, MessageType, 0, 0, *secondIntercomm, MPI_STATUS_IGNORE
    );
    printMessage("(Recv) 1|Third  <- 0|Second:", &message);
  }

  Manager_Finalize(manager);
  MPI_Finalize();
}