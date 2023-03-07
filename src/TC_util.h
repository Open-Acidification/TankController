#pragma once
#include <Arduino.h>

#if defined(__CYGWIN__)
extern size_t strnlen(const char *s, size_t n);
#endif

#if defined(ARDUINO_CI_COMPILATION_MOCKS)
#include <cassert>  // to support testing
#else
#define assert(p) (void)0
#endif

#if !defined(DEBUG)
#define DEBUG 0
#endif
#if DEBUG
#define COUT(x)                                                          \
  do {                                                                   \
    std::cout << __FILE__ << ":" << __LINE__ << " - " << x << std::endl; \
  } while (0)
#else
#define COUT(x) (void)0
#endif

int strscpy(char *destination, const char *source, unsigned long sizeOfDestination);
int strscpy_P(char *destination, const char *source, unsigned long sizeOfDestination);
int floattostrf(double float_value, int min_width, int num_digits_after_decimal, char *buffer,
                unsigned long buffer_size);
