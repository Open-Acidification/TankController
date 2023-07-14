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
  PHCalibrationMid(TankController* tc) : PHCalibration(tc) {
  }
  const __FlashStringHelper* prompt() {
    return F("pH-Midpoint");
  };
protected:
  void setMidValue(float value);
};

/**
 * @brief midpoint for 1-point pH calibration
 * 
 */
class PHCalibrationMidOne : public PHCalibrationMid {
public:
  PHCalibrationMidOne(TankController* tc) : PHCalibrationMid(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationMidOne");
  }
  void setValue(float value);
};

/**
 * @brief midpoint for 2-point pH calibration
 * 
 */
class PHCalibrationMidTwo : public PHCalibrationMid {
public:
  PHCalibrationMidTwo(TankController* tc) : PHCalibrationMid(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationMidTwo");
  }
  void setValue(float value);
};

/**
 * @brief midpoint for 3-point pH calibration
 * 
 */
class PHCalibrationMidThree : public PHCalibrationMid {
public:
  PHCalibrationMidThree(TankController* tc) : PHCalibrationMid(tc) {
  }
  const __FlashStringHelper* name() {
    return F("PHCalibrationMidThree");
  }
  void setValue(float value);
};
