#include "TC_util.h"

#include <string.h>

// Example: strscpy(buffer, source, sizeof(buffer));
int strscpy(char *destination, const char *source, unsigned long sizeOfDestination) {
  unsigned long sourceLength = strnlen(source, sizeOfDestination);
  if (sourceLength < sizeOfDestination) {
    *((char *)memcpy(destination, source, sourceLength) + sourceLength) = '\0';
    return 0;
  } else {
    // TODO: Log a WARNING that a string was truncated
    // Put a null terminator in the final byte
    *((char *)memcpy(destination, source, sizeOfDestination - 1) + sizeOfDestination - 1) = '\0';
    return 1;
  }
}

// Example: strscpy(buffer, source, sizeof(buffer));
int strscpy_P(char *destination, const char *source, unsigned long sizeOfDestination) {
  unsigned long sourceLength = strnlen(source, sizeOfDestination);
  if (sourceLength < sizeOfDestination) {
    *((char *)memcpy_P(destination, source, sourceLength) + sourceLength) = '\0';
    return 0;
  } else {
    // TODO: Log a WARNING that a string was truncated
    // Put a null terminator in the final byte
    *((char *)memcpy_P(destination, source, sizeOfDestination - 1) + sizeOfDestination - 1) = '\0';
    return 1;
  }
}
