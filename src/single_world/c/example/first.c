#include "libMPMDManager.h"
#include "message.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  const MPMDManager* manager = Manager_Init(argv);
  int localRank = Manager_Local_rank(manager);

  char connectionName[10];
  ConnectionId nameId = {.name = connectionName};

  Message message;
  MPI_Datatype MessageType;
  getMessageType(&MessageType);

  if(localRank == 0) {
    strcpy(connectionName, "second");
    const MPI_Comm* secondComm = Manager_Comm_to(manager, nameId, NAME_ID);

    fillMessage(&message, "0|First  to 1|Second", 1021);
    MPI_Send(&message, 1, MessageType, 1, 0, *secondComm);
    printMessage("(Send) 0|First  -> 1|Second:", &message);
  } else if(localRank == 1) {
    strcpy(connectionName, "third");
    const MPI_Comm* thirdComm = Manager_Comm_to(manager, nameId, NAME_ID);

    MPI_Recv(&message, 1, MessageType, 0, 0, *thirdComm, MPI_STATUS_IGNORE);
    printMessage("(Recv) 1|First  <- 0|Third: ", &message);
  }

  Manager_Finalize(manager);
  MPI_Finalize();
}