#include <stdio.h>

#include "mpmdManager.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  printf("Hello Third!\n");

  MPI_Finalize();
}