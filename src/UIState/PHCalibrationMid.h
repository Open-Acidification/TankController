/**
 * PHCalibrationMid.h
 *
 * pH midpoint calibration
 */
#pragma once
#include "PHCalibration.h"
#include "model/TC_util.h"  // For strnlen

/**
 * @brief Set mid/neutral buffer pH value as part of a 3-point calibration
 *
 */
class PHCalibrationMid : public PHCalibration {
public:
  const __FlashStringHelper* name() override {
    return F("PHCalibrationMid");
  }
  const __FlashStringHelper* prompt() override {
    return F("Mid buffer pH");
  };
  void setValue(float value) override;
};

/**
 * @brief Set mid/neutral buffer pH value as part of a 2-point calibration. We're using the comparative "higher" for the
 * sake of students who have trouble remembering which of two buffers is the "mid" one.
 *
 */
class PHCalibrationHigher : public PHCalibration {
public:
  const __FlashStringHelper* name() override {
    return F("PHCalibrationHigher");
  }
  const __FlashStringHelper* prompt() override {
    return F("Higher buffer pH");
  };
  void setValue(float value) override;
};

/**
 * @brief Set buffer pH value for a 1-point calibration.
 *
 */
class PHCalibrationOnly : public PHCalibration {
public:
  const __FlashStringHelper* name() override {
    return F("PHCalibrationOnly");
  }
  const __FlashStringHelper* prompt() override {
    return F("Buffer pH");
  };
  void setValue(float value) override;
};
