#include "utils.h"

bool streql(const char* first, const char* second) {
  if(first == NULL || second == NULL) return false;
  return strcmp(first, second) == 0;
}