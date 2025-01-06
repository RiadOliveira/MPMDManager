#include "libClientManager.h"
#include "libServerManager.h"
#include "message.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  Message message;
  MPI_Datatype MessageType;
  getMessageType(&MessageType);

  if(rank == 0) {
    const ServerManager* manager = Server_Init(argv, NULL, 1U);
    Server_Open(manager);
    const MPI_Comm* thirdComm = Server_Accept(manager, NULL);

    fillMessage(&message, "0|Second to 1|Third", 2031);
    MPI_Send(&message, 1, MessageType, 0, 0, *thirdComm);
    printMessage("(Send) 0|Second -> 1|Third: ", &message);

    Server_Finalize(manager);
  } else if(rank == 1) {
    const ClientManager* manager = Client_Init(1U);
    const MPI_Comm* firstComm = Client_Connect(manager, "first0", NULL);

    MPI_Recv(&message, 1, MessageType, 0, 0, *firstComm, MPI_STATUS_IGNORE);
    printMessage("(Recv) 1|Second <- 0|First: ", &message);

    Client_Finalize(manager);
  }

  MPI_Finalize();
}