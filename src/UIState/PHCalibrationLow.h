/**
 * PHCalibrationLow.h
 *
 * pH lowpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"  // For strnlen

/**
 * @brief Set low/acidic buffer pH value as part of a 3-point calibration
 *
 */
class PHCalibrationLow : public PHCalibration {
public:
  PHCalibrationLow(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() override {
    return F("PHCalibrationLow");
  }
  const __FlashStringHelper* prompt() override {
    return F("Low buffer pH");
  };
  void setValue(float value) override;
};

/**
 * @brief Set low/acidic buffer pH value as part of a 2-point calibration
 *
 */
class PHCalibrationLower : public PHCalibration {
public:
  PHCalibrationLower(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* name() override {
    return F("PHCalibrationLower");
  }
  const __FlashStringHelper* prompt() override {
    return F("Lower buffer pH");
  };
  void setValue(float value) override;
};
