/**
 * PHCalibrationHigh.h
 *
 * pH highpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "model/TC_util.h"  // For strnlen

/**
 * @brief Set high/basic buffer pH value as part of a 3-point calibration
 *
 */
class PHCalibrationHigh : public PHCalibration {
public:
  const __FlashStringHelper* name() override {
    return F("PHCalibrationHigh");
  }
  const __FlashStringHelper* prompt() override {
    return F("High buffer pH");
  };
  void setValue(float value) override;
};
