/**
 * PHCalibrationMid.h
 *
 * pH midpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "TC_util.h"  // For strnlen

class PHCalibrationMid : public PHCalibration {
public:
  PHCalibrationMid(TankController* tc, int numberOfCalibrationPoints = 3) : PHCalibration(tc) {
    this->numberOfCalibrationPoints = numberOfCalibrationPoints;
  }
  const __FlashStringHelper* name() override {
    return F("PHCalibrationMid");
  }
  const __FlashStringHelper* prompt() override {
    return F("pH-Midpoint");
  };
  void setValue(float value) override;

private:
  int numberOfCalibrationPoints;
};
