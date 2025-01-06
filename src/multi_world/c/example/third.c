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
    const MPI_Comm* firstComm = Server_Accept(manager, NULL);

    fillMessage(&message, "0|Third  to 1|First", 3011);
    MPI_Send(&message, 1, MessageType, 0, 0, *firstComm);
    printMessage("(Send) 0|Third  -> 1|First: ", &message);

    Server_Finalize(manager);
  } else if(rank == 1) {
    const ClientManager* manager = Client_Init(1U);
    const MPI_Comm* secondComm = Client_Connect(manager, "second0", NULL);

    MPI_Recv(&message, 1, MessageType, 0, 0, *secondComm, MPI_STATUS_IGNORE);
    printMessage("(Recv) 1|Third  <- 0|Second:", &message);

    Client_Finalize(manager);
  }

  MPI_Finalize();
}