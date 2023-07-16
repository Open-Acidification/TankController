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
  bool isInCalibration() override {
    return true;
  }
  void loop() override;
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeePHCalibration");
  }
  const __FlashStringHelper* prompt() override {
    return F("PH Calibration:");
  };
};
