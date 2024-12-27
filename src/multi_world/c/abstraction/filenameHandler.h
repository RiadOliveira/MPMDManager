#ifndef FILENAME_HANDLER
#define FILENAME_HANDLER

#include "shortcutTypes.h"

#define DOT_CHAR '.'
#define PATH_SEPARATOR '/'
#define NULL_TERMINATOR '\0'

#define NAME_MAX_SIZE 256

void getFilename(char* filename, uint* size, char** argv);

#endif