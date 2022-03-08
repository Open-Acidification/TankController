#pragma once
#include <Arduino.h>

#include "../TankController.h"

/**
 * Temperature Control
 * Controlling the tank temperature is done by external devices that are controlled by setting
 * designated pins on the Arduino.
 */

class TemperatureControl {
private:
  static TemperatureControl* _instance;

protected:
  const uint16_t TEMP_CONTROL_PIN = 47;
  const float DELTA = 0.05;
  uint32_t lastSwitchMS = 0;
  float targetTemperature;
  float currentTemperatureTarget;
  float rampStartingTemp;
  uint32_t rampTimeStart;
  uint32_t rampTimeEnd;
  float amplitude;
  uint32_t period;
  uint32_t sineStartTime;
  int tempSetType = FLAT_TYPE;
  TemperatureControl();

public:
  enum tempSetTypeTypes {
    FLAT_TYPE,
    RAMP_TYPE,
    SINE_TYPE,
  };
  virtual ~TemperatureControl() {
  }
  static TemperatureControl* instance();
  static void clearInstance();
  static void enableHeater(bool flag);
  float getTargetTemperature() {
    return targetTemperature;
  }
  float getCurrentTemperatureTarget() {
    return currentTemperatureTarget;
  }
  int getTempSetType() {
    return tempSetType;
  }
  float getAmplitude() {
    return amplitude;
  }
  uint32_t getRampTimeStart() {
    return rampTimeStart;
  }
  uint32_t getRampTimeEnd() {
    return rampTimeEnd;
  }
  uint32_t getPeriod() {
    return period;
  }
  virtual bool isHeater();
  bool isOn();
  void setTargetTemperature(float newTemperature);
  void setRampDuration(float newTempRampDuration);
  void setSine(float sineAmplitude, float sinePeriodInHours);
  virtual void updateControl(float currentTemperature) = 0;
};

class Heater : public TemperatureControl {
public:
  Heater() : TemperatureControl(){};
  bool isHeater() {
    return true;
  }
  void updateControl(float currentTemperature);
};

class Chiller : public TemperatureControl {
private:
  const uint32_t TIME_INTERVAL = 30 * 1000UL;  // interval at which to change chiller state
  uint32_t previousMillis = 0;                 // will store last time chiller state was checked

public:
  Chiller() : TemperatureControl(){};
  bool isHeater() {
    return false;
  }
  void updateControl(float currentTemperature);
};
