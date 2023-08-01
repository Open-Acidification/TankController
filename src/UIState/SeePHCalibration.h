/**
 * SeePHCalibration.h
 *
 * Display the pH calibration
 */
#pragma once
#include "UIState.h"

class SeePHCalibration : public UIState {
public:
  SeePHCalibration(bool inCalibration = false);
  bool isInCalibration() override {
    return inCalibration;
  }
  void loop() override;
  void start() override;
  const __FlashStringHelper* name() override {
    return F("SeePHCalibration");
  }
  const __FlashStringHelper* prompt() override {
    return F("PH Calibration:");
  };

private:
  uint32_t endTime = 0;
  bool inCalibration;
};
