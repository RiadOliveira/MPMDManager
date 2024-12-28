#include "error.h"

inline void exitWithError(const char* format, ...) {
  va_list messages;

  va_start(messages, format);
  vfprintf(stderr, format, messages);
  fprintf(stderr, "\n");
  va_end(messages);

  MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
}