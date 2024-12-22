#include "filenameHandler.h"

void getFilename(char* filename, uint* size, char** argv) {
  char currentChar;
  const char* argChar = argv[0];
  uint filenameInd = 0, lastDotInd = 0;

  while((currentChar = *(argChar++)) != NULL_TERMINATOR) {
    if(currentChar == PATH_SEPARATOR) filenameInd = 0;
    else {
      if(currentChar == DOT_CHAR) lastDotInd = filenameInd;
      filename[filenameInd++] = currentChar;
    }
  }

  const bool hasDot = lastDotInd > 0;
  filename[hasDot ? lastDotInd : filenameInd] = NULL_TERMINATOR;
  *size = filenameInd + hasDot;
}