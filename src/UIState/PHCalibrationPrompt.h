/**
 * PHCalibrationPrompt.h
 *
 * Choose 1-point, 2-point, or 3-point pH calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"  // For strnlen

class PHCalibrationPrompt : public PHCalibration {
public:
  const __FlashStringHelper* name() override {
    return F("PHCalibrationPrompt");
  }
  const __FlashStringHelper* prompt() override {
    return F("1, 2 or 3 point?");
  };
  void setValue(float value) override;

private:
  bool isInteger() override {
    return true;
  }
};
