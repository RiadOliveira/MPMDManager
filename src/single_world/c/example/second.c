#include "mpmdManager.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  const MPMDManager* manager = Manager_Init(argv);

  Manager_Finalize(manager);
  MPI_Finalize();
}