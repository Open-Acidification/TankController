/**
 * SetTime.h
 *
 * Manually set the Arduino date and time
 */
#pragma once
#include "NumberCollectorState.h"

class SetTime : public NumCollectorState {
public:
  SetTime(TankControllerLib* tc);
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return (const char*)F("SetTime");
  }
  float getCurrentValue();

  const char* prompt() {
    return prompts[subState];
  }
  void setValue(float value);

private:
  static const uint16_t NUM_VALUES = 5;
  uint16_t subState = 0;
  uint16_t values[NUM_VALUES] = {0, 0, 0, 0, 0};
  float current[NUM_VALUES] = {0.0, 0.0, 0.0, 0.0, 0.0};
  const char* prompts[NUM_VALUES];
};
