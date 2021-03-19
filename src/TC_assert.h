#pragma once
#include <Arduino.h>

#ifdef MOCK_PINS_COUNT
#include <cassert>  // to support testing
#else
#define assert(p) (void)0
#endif
