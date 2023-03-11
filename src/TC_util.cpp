#include "TC_util.h"

#include <string.h>

#include "Devices/Serial_TC.h"

// Example: strscpy(buffer, source, sizeof(buffer));
int strscpy(char *destination, const char *source, unsigned long sizeOfDestination) {
  unsigned long sourceLength = strnlen(source, sizeOfDestination);
  if (sourceLength < sizeOfDestination) {
    // Put a null terminator in the final byte
    *((char *)memcpy(destination, source, sourceLength) + sourceLength) = '\0';
    return 0;
  } else {
    // Put a null terminator in the final byte
    *((char *)memcpy(destination, source, sizeOfDestination - 1) + sizeOfDestination - 1) = '\0';
    serial(F("WARNING! String \"%s\" was truncated to \"%s\""), source, destination);
    // TODO: Log a WARNING that a string was truncated
    return 1;
  }
}

// In this function, the source is treated as an address in PROGMEM, not RAM
// Example: strscpy_P(buffer, F("some string here"), sizeof(buffer));
int strscpy_P(char *destination, const __FlashStringHelper *source_F, unsigned long sizeOfDestination) {
  PGM_P source = (PGM_P)source_F;
  unsigned long sourceLength = strlen_P(source);
  if (sourceLength < sizeOfDestination) {
    // Put a null terminator in the final byte
    *((char *)memcpy_P(destination, source, sourceLength) + sourceLength) = '\0';
    return 0;
  } else {
    // Put a null terminator in the final byte
    *((char *)memcpy_P(destination, source, sizeOfDestination - 1) + sizeOfDestination - 1) = '\0';
    // TODO: Log a WARNING that a string was truncated
    serial(F("WARNING! String (P) \"%s\" was truncated to \"%s\""), (PGM_P)source, destination);
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
  } else if (strnlen(large_buffer, sizeof(large_buffer)) < sizeof(large_buffer) - 1) {
    strscpy(buffer, large_buffer, buffer_size);
    return 1;
  } else {
    serial(F("WARNING! Memory overflow may have occurred"), buffer);
    strscpy(buffer, large_buffer, buffer_size);
    // TODO: Log a WARNING that buffer overflow might have occurred for number large_buffer
    return 2;
  }
}
