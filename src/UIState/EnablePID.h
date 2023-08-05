/**
 * EnablePID.h
 *
 * Enable PID Controller
 */
#pragma once
#include "NumberCollectorState.h"

class EnablePID : public NumberCollectorState {
public:
  virtual bool isInteger() {
    return true;
  }
  const __FlashStringHelper* name() {
    return F("EnablePID");
  }
  float getCurrentValue();
  const __FlashStringHelper* prompt() {
    return F("PID 1:on; 9:off");
  };
  void setValue(float value);
};
