/**
 * PHCalibrationHigh.h
 *
 * pH highpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"  // For strnlen

class PHCalibrationHigh : public PHCalibration {
public:
  PHCalibrationHigh(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() override {
    return F("PHCalibrationHigh");
  }
  const __FlashStringHelper* prompt() override {
    return F("pH-Highpoint");
  };
  void setValue(float value) override;
};
