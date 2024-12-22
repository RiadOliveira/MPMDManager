#include "mpmdManager.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  Manager_Init(argv);

  MPI_Finalize();
}