/**
 * SeePHCalibration.h
 *
 * Display the pH calibration
 */
#pragma once
#include "UIState.h"

class SeePHCalibration : public UIState {
public:
  SeePHCalibration(TankController* tc) : UIState(tc) {
  }
  void start() override;
  void loop() override;
  const __FlashStringHelper* name() override {
    return F("SeePHCalibration");
  }
  const __FlashStringHelper* prompt() override {
    return F("PH Calibration:");
  };
};
