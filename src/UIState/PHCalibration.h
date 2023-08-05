#pragma once
#include "Devices/PHProbe.h"
#include "NumberCollectorState.h"
#include "SeePHCalibration.h"

class PHCalibration : public NumberCollectorState {
public:
  bool isInCalibration() override {
    return true;  // disable controls during calibration
  }
  float getCurrentValue() override {
    return 0;
  }
  void handleKey(char key) override {
    if (key == 'D') {  // cancel but show calibration status
      this->setNextState(new SeePHCalibration(true));
    } else {
      NumberCollectorState::handleKey(key);
    }
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
