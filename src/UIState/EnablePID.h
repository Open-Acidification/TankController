/**
 * EnablePID.h
 *
 * Enable PID Controller
 */
#pragma once
#include "NumberCollectorState.h"
#include "TC_util.h"

class EnablePID : public NumCollectorState {
public:
  EnablePID(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInteger() {
    return true;
  }
  const char* name() {
    return CSTR("EnablePID");
  }
  float getCurrentValue();
  const char* prompt() {
    return CSTR("PID 1:on; 9:off");
  };
  void setValue(float value);
};
