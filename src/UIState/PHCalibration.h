#pragma once
#include "Devices/PHProbe.h"
#include "NumberCollectorState.h"

class PHCalibration : public NumCollectorState {
public:
  PHCalibration(TankControllerLib* tc) : NumCollectorState(tc) {
  }
  virtual bool isInCalibration() {
    return true;  // disable controls during calibration
  }
  float getCurrentValue() {
    return PHProbe::instance()->getPh();
  }
  uint16_t getCurrentValuePrecision() {
    return 3;
  }
  void loop() {
    printValue();
  }
};
