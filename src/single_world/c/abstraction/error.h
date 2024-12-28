#ifndef ERROR
#define ERROR

#include <mpi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void exitWithError(const char* format, ...);

#endif