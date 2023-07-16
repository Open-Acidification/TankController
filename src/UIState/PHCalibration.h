#pragma once
#include "Devices/PHProbe.h"
#include "NumberCollectorState.h"

class PHCalibration : public NumCollectorState {
public:
  PHCalibration(TankController* tc) : NumCollectorState(tc) {
  }
  bool isInCalibration() override {
    return true;  // disable controls during calibration
  }
  float getCurrentValue() override {
    return 0;
  }
  void loop() override {
    printValue();
  }

protected:
  uint16_t getCurrentValuePrecision() override {
    return 3;
  }
  bool showCurrentValue() override {
    return false;
  }
};
