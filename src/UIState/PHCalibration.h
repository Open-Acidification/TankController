#pragma once
#include "Devices/PHProbe.h"
#include "NumberCollectorState.h"

class PHCalibration : public NumCollectorState {
public:
  PHCalibration(TankController* tc) : NumCollectorState(tc) {
  }
  bool isInCalibration() {
    return true;  // disable controls during calibration
  }
  float getCurrentValue() {
    return 0;
  }
  void loop() {
    printValue();
  }

protected:
  uint16_t getCurrentValuePrecision() {
    return 3;
  }
  bool showCurrentValue() {
    return false;
  }
};
