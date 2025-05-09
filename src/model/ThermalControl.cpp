#include "model/ThermalControl.h"

#include "TankController.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/Serial_TC.h"
#include "wrappers/ThermalProbe_TC.h"

const float DEFAULT_TEMPERATURE = 20.0;

//  class instance variables
/**
 * static variable for singleton
 */
ThermalControl *ThermalControl::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
ThermalControl *ThermalControl::instance() {
  if (!_instance) {
    if (EEPROM_TC::instance()->getHeat()) {
      _instance = new Heater();
    } else {
      _instance = new Chiller();
    }
  }
  return _instance;
}

void ThermalControl::clearInstance() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
}

void ThermalControl::enableHeater(bool flag) {
  EEPROM_TC::instance()->setHeat(flag);
  if (_instance && (_instance->isHeater() != flag)) {
    delete _instance;
    _instance = nullptr;
    char buffer[7];
    strscpy_P(buffer, (flag ? F("true)") : F("false)")), sizeof(buffer));
    serial(F("ThermalControl::enableHeater(%s"), buffer);
  }
}

/**
 * protected constructor
 */
ThermalControl::ThermalControl() {
  baseThermalTarget = EEPROM_TC::instance()->getThermalTarget();
  if (isnan(baseThermalTarget)) {
    baseThermalTarget = DEFAULT_TEMPERATURE;
    EEPROM_TC::instance()->setTemp(baseThermalTarget);
  }
  uint16_t thermalFunctionInt = EEPROM_TC::instance()->getThermalFunctionType();
  if (thermalFunctionInt == 0xFFFF) {
    thermalFunctionType = FLAT_TYPE;
    EEPROM_TC::instance()->setThermalFunctionType(thermalFunctionType);
  } else {
    thermalFunctionType = (thermalFunctionTypes)thermalFunctionInt;
  }
  switch (thermalFunctionType) {
    case RAMP_TYPE:
      rampTimeEndSeconds = EEPROM_TC::instance()->getThermalRampTimeEnd();
      if (rampTimeEndSeconds == 0xFFFFFFFF || rampTimeEndSeconds == 0) {
        rampTimeEndSeconds = 0;
        EEPROM_TC::instance()->setThermalRampTimeEnd(rampTimeEndSeconds);
        rampTimeStartSeconds = 0;
        EEPROM_TC::instance()->setThermalRampTimeStart(rampTimeStartSeconds);
      } else {
        rampTimeStartSeconds = EEPROM_TC::instance()->getThermalRampTimeStart();
        rampInitialValue = EEPROM_TC::instance()->getThermalRampInitialValue();
      }
      break;
    case SINE_TYPE:
      periodInSeconds = EEPROM_TC::instance()->getThermalSinePeriod();
      amplitude = EEPROM_TC::instance()->getThermalSineAmplitude();
      sineStartTime = EEPROM_TC::instance()->getThermalSineStartTime();
      break;
    default:
      break;
  }
  pinMode(THERMAL_CONTROL_PIN, OUTPUT);
  digitalWrite(THERMAL_CONTROL_PIN, TURN_SOLENOID_OFF);
  char buffer1[8];
  char buffer2[10];
  strscpy_P(buffer1, (this->isHeater() ? F("Heater") : F("Chiller")), sizeof(buffer1));
  floattostrf(baseThermalTarget, 5, 2, buffer2, sizeof(buffer2));
  serial(F("%s starts with solenoid off with target temperature of %s C"), buffer1, buffer2);
}

void ThermalControl::setRampDurationHours(float newThermalRampDuration) {
  if (newThermalRampDuration > 0) {
    float currentRampTime = rampTimeEndSeconds - rampTimeStartSeconds;
    char buffer1[10];
    char buffer2[10];
    floattostrf(currentRampTime, 5, 3, buffer1, sizeof(buffer1));
    floattostrf(newThermalRampDuration, 5, 3, buffer2, sizeof(buffer2));
    serial(F("change ramp time from %s to %s"), buffer1, buffer2);
    rampTimeStartSeconds = DateTime_TC::now().secondstime();
    rampTimeEndSeconds = rampTimeStartSeconds + (uint32_t)(newThermalRampDuration * 3600);
    rampInitialValue = ThermalProbe_TC::instance()->getRunningAverage();
    thermalFunctionType = thermalFunctionTypes::RAMP_TYPE;
    EEPROM_TC::instance()->setThermalFunctionType(thermalFunctionType);
    EEPROM_TC::instance()->setThermalRampTimeStart(rampTimeStartSeconds);
    EEPROM_TC::instance()->setThermalRampTimeEnd(rampTimeEndSeconds);
    EEPROM_TC::instance()->setThermalRampInitialValue(rampInitialValue);
  } else {
    rampTimeEndSeconds = 0;
    thermalFunctionType = thermalFunctionTypes::FLAT_TYPE;
    EEPROM_TC::instance()->setThermalFunctionType(thermalFunctionType);
    EEPROM_TC::instance()->setThermalRampTimeEnd(rampTimeEndSeconds);
    serial("set ramp time to 0");
  }
}

void ThermalControl::setSineAmplitudeAndHours(float sineAmplitude, float sinePeriodInHours) {
  periodInSeconds = (sinePeriodInHours * 3600);
  amplitude = sineAmplitude;
  thermalFunctionType = thermalFunctionTypes::SINE_TYPE;
  sineStartTime = DateTime_TC::now().secondstime();
  EEPROM_TC::instance()->setThermalFunctionType(thermalFunctionType);
  EEPROM_TC::instance()->setThermalSinePeriod(periodInSeconds);
  EEPROM_TC::instance()->setThermalSineAmplitude(amplitude);
  EEPROM_TC::instance()->setThermalSineStartTime(sineStartTime);
}

void ThermalControl::setSineAmplitude(float sineAmplitude) {
  assert(thermalFunctionType == thermalFunctionTypes::SINE_TYPE);
  amplitude = sineAmplitude;
  EEPROM_TC::instance()->setThermalSineAmplitude(amplitude);
}

/**
 * is heater
 */
bool ThermalControl::isHeater() {
  return true;
}

bool ThermalControl::isOn() {
  return digitalRead(THERMAL_CONTROL_PIN) == TURN_SOLENOID_ON;
}

/**
 * set target temperature and save in EEPROM
 */
void ThermalControl::setThermalTarget(float newTemperature) {
  if (baseThermalTarget != newTemperature) {
    char buffer1[10];
    char buffer2[10];
    floattostrf(baseThermalTarget, 5, 2, buffer1, sizeof(buffer1));
    floattostrf(newTemperature, 5, 2, buffer2, sizeof(buffer2));
    serial(F("change target temperature from %s to %s"), buffer1, buffer2);
    EEPROM_TC::instance()->setTemp(newTemperature);
    baseThermalTarget = newTemperature;
  }
}

void Chiller::updateControl(float currentTemperature) {
  // called each loop()
  uint32_t currentMillis = millis();
  uint32_t currentTime = DateTime_TC::now().secondstime();
  switch (thermalFunctionType) {
    case FLAT_TYPE: {
      currentThermalTarget = baseThermalTarget;
      break;
    }
    case RAMP_TYPE: {
      if (currentTime < rampTimeEndSeconds) {
        currentThermalTarget =
            rampInitialValue + ((currentTime - rampTimeStartSeconds) * (baseThermalTarget - rampInitialValue) /
                                (rampTimeEndSeconds - rampTimeStartSeconds));
      } else {
        currentThermalTarget = baseThermalTarget;
      }
      break;
    }
    case SINE_TYPE: {
      uint32_t sineEndTime = sineStartTime + periodInSeconds;
      if (currentTime >= sineEndTime) {
        sineStartTime = DateTime_TC::now().secondstime();
        sineEndTime = sineStartTime + periodInSeconds;
        EEPROM_TC::instance()->setThermalSineStartTime(sineStartTime);
      }
      float timeLeftTillPeriodEnd = sineEndTime - currentTime;
      float percentNOTThroughPeriod = timeLeftTillPeriodEnd / periodInSeconds;
      float percentThroughPeriod = 1 - percentNOTThroughPeriod;
      float x = percentThroughPeriod * (2 * PI);                      // the x position for our sine wave
      currentThermalTarget = amplitude * sin(x) + baseThermalTarget;  // y position in our sine wave
      break;
    }
    default:
      break;
  }
  if (currentMillis < previousMillis) {
    previousMillis = 0;  // reset if clock went backwards (typical during tests)
  }
  // pause 30 seconds between switching chiller on and off to prevent damage to chiller
  if (currentMillis - previousMillis < TIME_INTERVAL) {
  } else {
    bool oldValue = digitalRead(THERMAL_CONTROL_PIN);
    bool newValue;
    previousMillis = currentMillis;
    // if in calibration, turn unit off
    if (TankController::instance()->isInCalibration()) {
      newValue = TURN_SOLENOID_OFF;
    }
    // if the observed temperature is above the set-point range turn on the chiller
    else if (currentTemperature >= currentThermalTarget + DELTA) {
      newValue = TURN_SOLENOID_ON;
    }
    // if the observed temperature is below the set-point range turn off the chiller
    else if (currentTemperature <= currentThermalTarget - DELTA) {
      newValue = TURN_SOLENOID_OFF;
    } else {
      newValue = oldValue;
    }
    if (newValue != oldValue) {
      uint32_t currentMS = millis();
      serial(F("chiller turned %s at %lu after %lu ms"), newValue ? "off" : "on", currentMS, currentMS - lastSwitchMS);
      lastSwitchMS = currentMS;
      digitalWrite(THERMAL_CONTROL_PIN, newValue);
    }
  }
}

void Heater::updateControl(float currentTemperature) {
  // called each loop()
  uint32_t currentTime = DateTime_TC::now().secondstime();
  switch (thermalFunctionType) {
    case FLAT_TYPE: {
      currentThermalTarget = baseThermalTarget;
      break;
    }
    case RAMP_TYPE: {
      if (currentTime < rampTimeEndSeconds) {
        currentThermalTarget =
            rampInitialValue + ((currentTime - rampTimeStartSeconds) * (baseThermalTarget - rampInitialValue) /
                                (rampTimeEndSeconds - rampTimeStartSeconds));
      } else {
        currentThermalTarget = baseThermalTarget;
      }
      break;
    }
    case SINE_TYPE: {
      uint32_t sineEndTime = sineStartTime + periodInSeconds;
      if (currentTime >= sineEndTime) {
        sineStartTime = DateTime_TC::now().secondstime();
        sineEndTime = sineStartTime + periodInSeconds;
        EEPROM_TC::instance()->setThermalSineStartTime(sineStartTime);
      }
      float timeLeftTillPeriodEnd = sineEndTime - currentTime;
      float percentNOTThroughPeriod = timeLeftTillPeriodEnd / periodInSeconds;
      float percentThroughPeriod = 1 - percentNOTThroughPeriod;
      float x = percentThroughPeriod * (2 * PI);                      // the x position for our sine wave
      currentThermalTarget = amplitude * sin(x) + baseThermalTarget;  // y position in our sine wave
      break;
    }
    default:
      break;
  }
  bool oldValue = digitalRead(THERMAL_CONTROL_PIN);
  bool newValue;
  // if in calibration, turn unit off
  if (TankController::instance()->isInCalibration()) {
    newValue = TURN_SOLENOID_OFF;
  }
  // if the observed temperature is below the temperature set-point range turn on the heater
  else if (currentTemperature <= currentThermalTarget - DELTA) {
    newValue = TURN_SOLENOID_ON;
  }
  // if the observed temperature is above the temperature set-point range turn off the heater
  else if (currentTemperature >= currentThermalTarget + DELTA) {
    newValue = TURN_SOLENOID_OFF;
  } else {
    newValue = oldValue;
  }
  if (newValue != oldValue) {
    uint32_t currentMS = millis();
    serial(F("heater turned %s at %lu after %lu ms"), newValue ? "off" : "on", currentMS, currentMS - lastSwitchMS);
    lastSwitchMS = currentMS;
    digitalWrite(THERMAL_CONTROL_PIN, newValue);
  }
}
