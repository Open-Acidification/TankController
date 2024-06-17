#pragma once
#include <Arduino.h>

#include "TankController.h"

/**
 * Temperature Control
 * Controlling the tank temperature is done by external devices that are controlled by setting
 * designated pins on the Arduino.
 */

class ThermalControl {
public:
  enum thermalFunctionTypes {
    FLAT_TYPE,
    RAMP_TYPE,
    SINE_TYPE,
  };

private:
  static ThermalControl* _instance;

protected:
  const uint16_t THERMAL_CONTROL_PIN = 47;
  const float DELTA = 0.05;
  uint32_t lastSwitchMS = 0;
  float baseThermalTarget;     // base target temperature
  float currentThermalTarget;  // current target (ramp, sine, arbitrary path)
  float rampInitialValue;
  uint32_t rampTimeStartSeconds;
  uint32_t rampTimeEndSeconds;
  float amplitude;
  uint32_t periodInSeconds;
  uint32_t sineStartTime;
  thermalFunctionTypes thermalFunctionType = FLAT_TYPE;
  ThermalControl();

public:
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
  thermalFunctionTypes getThermalFunctionType() {
    return thermalFunctionType;
  }
  float getAmplitude() {
    return amplitude;
  }
  uint32_t getRampTimeStart() {
    return thermalFunctionType == FLAT_TYPE ? 0 : rampTimeStartSeconds;
  }
  uint32_t getRampTimeEnd() {
    return thermalFunctionType == FLAT_TYPE ? 0 : rampTimeEndSeconds;
  }
  uint32_t getPeriodInSeconds() {
    return periodInSeconds;
  }
  virtual bool isHeater();
  bool isOn();
  void setThermalTarget(float newTemperature);
  void setRampDurationHours(float newThermalRampDuration);
  void setSineAmplitudeAndHours(float sineAmplitude, float sinePeriodInHours);
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
