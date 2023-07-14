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
  PHCalibrationLow(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* prompt() {
    return F("pH-Lowpoint");
  };

protected:
  void setLowValue(float value);
};

/**
 * @brief lowpoint for 2-point pH calibration
 *
 */
class PHCalibrationLowTwo : public PHCalibrationLow {
public:
  PHCalibrationLowTwo(TankController* tc) : PHCalibrationLow(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationLowTwo");
  }
  void setValue(float value);
};

/**
 * @brief lowpoint for 3-point pH calibration
 *
 */
class PHCalibrationLowThree : public PHCalibrationLow {
public:
  PHCalibrationLowThree(TankController* tc) : PHCalibrationLow(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationLowThree");
  }
  void setValue(float value);
};
