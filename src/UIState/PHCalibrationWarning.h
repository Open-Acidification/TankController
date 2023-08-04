/**
 * PHCalibrationWarning.h
 *
 * Warn that pH slope is out of normal range
 */
#pragma once
#include "UIState.h"

class PHCalibrationWarning : public UIState {
public:
  PHCalibrationWarning();
  void handleKey(char key) override;
  bool isInCalibration() override {
    return true;
  }
  void loop() override;
  const __FlashStringHelper* name() override {
    return F("PHCalibrationWarning");
  }
  const __FlashStringHelper* prompt() override {
    return F("BAD CALIBRATION?");
  };

private:
  uint32_t startTime = 0;
};
