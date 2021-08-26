/**
 * EnablePID.h
 *
 * Enable PID Controller
 */
#pragma once
#include "NumberCollectorState.h"

class EnablePID : public NumCollectorState {
public:
  EnablePID(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return (const char*)F("EnablePID");
  }
  float getCurrentValue();
  const char* prompt() {
    return (const char*)F("PID 1:on; 9:off");
  };
  void setValue(float value);
};
