#pragma once
#include <Arduino.h>

#include "../TankController.h"

/**
 * Temperature Control
 * Controlling the tank temperature is done by external devices that are controlled by setting
 * designated pins on the Arduino.
 */

class ThermalControl {
private:
  static ThermalControl* _instance;

protected:
  const uint16_t TEMP_CONTROL_PIN = 47;
  const float DELTA = 0.05;
  uint32_t lastSwitchMS = 0;
  float baseThermalTarget;     // base target
  float currentThermalTarget;  // current target (ramp, sine, arbitrary path)
  float rampStartingTemp;
  uint32_t rampTimeStart;
  uint32_t rampTimeEnd;
  float amplitude;
  uint32_t period;
  uint32_t sineStartTime;
  int tempSetType = FLAT_TYPE;
  ThermalControl();

public:
  enum tempSetTypeTypes {
    FLAT_TYPE,
    RAMP_TYPE,
    SINE_TYPE,
  };
  virtual ~ThermalControl() {
  }
  static ThermalControl* instance();
  static void clearInstance();
  static void enableHeater(bool flag);
  float getBaseThermalTarget() {
    return baseThermalTarget;
  }
  float getCurrentThermalTarget() {
    return currentThermalTarget;
  }
  int getTempSetType() {
    return tempSetType;
  }
  float getAmplitude() {
    return amplitude;
  }
  uint32_t getRampTimeStart() {
    return tempSetType == FLAT_TYPE ? 0 : rampTimeStart;
  }
  uint32_t getRampTimeEnd() {
    return tempSetType == FLAT_TYPE ? 0 : rampTimeEnd;
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

class Heater : public ThermalControl {
public:
  Heater() : ThermalControl(){};
  bool isHeater() {
    return true;
  }
  void updateControl(float currentTemperature);
};

class Chiller : public ThermalControl {
private:
  const uint32_t TIME_INTERVAL = 30 * 1000UL;  // interval at which to change chiller state
  uint32_t previousMillis = 0;                 // will store last time chiller state was checked

public:
  Chiller() : ThermalControl(){};
  bool isHeater() {
    return false;
  }
  void updateControl(float currentTemperature);
};
