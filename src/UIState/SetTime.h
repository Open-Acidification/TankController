/**
 * SetTime.h
 *
 * Manually set the Arduino date and time
 */
#pragma once
#include "NumberCollectorState.h"

class SetTime : public NumCollectorState {
public:
  SetTime(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  const char* prompt() {
    return prompts[subState];
  }
  void setValue(double value);

private:
  static const int NUM_VALUES = 5;
  int subState = 0;
  int values[NUM_VALUES] = {0, 0, 0, 0, 0};
  const char* prompts[NUM_VALUES] = {
      "Set Year (YYYY):", "Month (1-12):   ", "Day (1-31):     ", "Hour (0-23):    ", "Minute (0-59):  "};
};
