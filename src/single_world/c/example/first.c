#include <stdio.h>

#include "mpmdManager.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  printf("Hello First!\n");

  MPI_Finalize();
}