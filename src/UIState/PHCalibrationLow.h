/**
 * PHCalibrationLow.h
 *
 * pH lowpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"  // For strnlen

class PHCalibrationLow : public PHCalibration {
public:
  PHCalibrationLow(TankController* tc, int numberOfCalibrationPoints = 3) : PHCalibration(tc) {
    this->numberOfCalibrationPoints = numberOfCalibrationPoints;
  }
  const __FlashStringHelper* name() override {
    return F("PHCalibrationLow");
  }
  const __FlashStringHelper* prompt() override {
    return F("pH-Lowpoint");
  };

private:
  int numberOfCalibrationPoints;
  void setValue(float value) override;
};
