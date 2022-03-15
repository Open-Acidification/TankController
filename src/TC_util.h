#pragma once
#include <Arduino.h>

#if defined(__CYGWIN__)
  extern size_t strnlen (const char* s, size_t n);
#endif

#ifdef MOCK_PINS_COUNT
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
