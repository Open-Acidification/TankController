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

// Write the float to the string buffer, testing for overflow.
// Example: floattostrf(number, 6, 2, buffer, sizeof(buffer));
int floattostrf(double float_value, int min_width, int num_digits_after_decimal, char *buffer,
                unsigned long buffer_size) {
  char large_buffer[buffer_size + 10];
  dtostrf(float_value, min_width, num_digits_after_decimal, large_buffer);
  if (strnlen(large_buffer, sizeof(large_buffer)) < buffer_size) {
    strscpy(buffer, large_buffer, buffer_size);
    return 0;
  } else if (strnlen(large_buffer, sizeof(large_buffer)) < sizeof(large_buffer)) {
    strscpy(buffer, large_buffer, buffer_size);
    // TODO: Log a WARNING that string large_buffer was truncated to buffer
    return 1;
  } else {
    strscpy(buffer, large_buffer, buffer_size);
    // TODO: Log a WARNING that buffer overflow may have occurred for number large_buffer
    return 2;
  }
}
