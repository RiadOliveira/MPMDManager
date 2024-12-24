#include "message.h"
#include "mpmdManager.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  const MPMDManager* manager = Manager_Init(argv);
  int localRank = Manager_Local_Rank(manager);

  char programName[10];
  ProgramIdentifier nameId = {.name = programName};

  Message message;
  MPI_Datatype MessageType;
  getMessageType(&MessageType);

  if(localRank == 0) {
    strcpy(programName, "first");
    const MPI_Comm* firstComm = Manager_Intercomm_to(manager, nameId, NAME_ID);

    fillMessage(&message, "0|Third  to 1|First", 3011);
    MPI_Send(&message, 1, MessageType, 1, 0, *firstComm);
    printMessage("(Send) 0|Third  -> 1|First: ", &message);
  } else if(localRank == 1) {
    strcpy(programName, "second");
    const MPI_Comm* secondComm = Manager_Intercomm_to(manager, nameId, NAME_ID);

    MPI_Recv(&message, 1, MessageType, 0, 0, *secondComm, MPI_STATUS_IGNORE);
    printMessage("(Recv) 1|Third  <- 0|Second:", &message);
  }

  Manager_Finalize(manager);
  MPI_Finalize();
}