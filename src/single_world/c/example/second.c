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
    strcpy(connectionName, "third");
    const MPI_Comm* thirdComm = Manager_Comm_to(manager, nameId, NAME_ID);

    fillMessage(&message, "0|Second to 1|Third", 2031);
    MPI_Send(&message, 1, MessageType, 1, 0, *thirdComm);
    printMessage("(Send) 0|Second -> 1|Third: ", &message);
  } else if(localRank == 1) {
    strcpy(connectionName, "first");
    const MPI_Comm* firstComm = Manager_Comm_to(manager, nameId, NAME_ID);

    MPI_Recv(&message, 1, MessageType, 0, 0, *firstComm, MPI_STATUS_IGNORE);
    printMessage("(Recv) 1|Second <- 0|First: ", &message);
  }

  Manager_Finalize(manager);
  MPI_Finalize();
}