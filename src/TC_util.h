#pragma once
#include <Arduino.h>

#ifdef MOCK_PINS_COUNT
#include <cassert>  // to support testing
#else
#define assert(p) (void)0
#endif

#define DEBUG 0
#if DEBUG
#define COUT(x)                                                          \
  do {                                                                   \
    std::cout << __FILE__ << ":" << __LINE__ << " - " << x << std::endl; \
  } while (0)
#else
#define COUT(x) (void)0
#endif
