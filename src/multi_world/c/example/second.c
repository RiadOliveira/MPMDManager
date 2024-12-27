#include "libServerManager.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  const ServerManager* manager = Server_Init(argv, NULL, 2);

  Server_Finalize(manager);
  MPI_Finalize();
}