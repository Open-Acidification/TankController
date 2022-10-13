#include "TemperatureControl.h"

#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/TempProbe_TC.h"
#include "Serial_TC.h"
#include "TankController.h"

const float DEFAULT_TEMPERATURE = 20.0;

//  class instance variables
/**
 * static variable for singleton
 */
TemperatureControl *TemperatureControl::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
TemperatureControl *TemperatureControl::instance() {
  if (!_instance) {
    if (EEPROM_TC::instance()->getHeat()) {
      _instance = new Heater();
    } else {
      _instance = new Chiller();
    }
  }
  return _instance;
}

void TemperatureControl::clearInstance() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
}

void TemperatureControl::enableHeater(bool flag) {
  EEPROM_TC::instance()->setHeat(flag);
  if (_instance && (_instance->isHeater() != flag)) {
    delete _instance;
    _instance = nullptr;
    char buffer[7];
    buffer = (flag ? (PGM_P)F("true)") : (PGM_P)F("false)"));
    serial(F("TemperatureControl::enableHeater(%s"), buffer);
  }
}

/**
 * protected constructor
 */
TemperatureControl::TemperatureControl() {
  COUT("TemperatureControl()");
  targetTemperature = EEPROM_TC::instance()->getTemp();
  if (isnan(targetTemperature)) {
    targetTemperature = DEFAULT_TEMPERATURE;
    EEPROM_TC::instance()->setTemp(targetTemperature);
  }
  tempSetType = EEPROM_TC::instance()->getTempSetType();
  if (tempSetType == 0xFFFFFFFF) {
    tempSetType = FLAT_TYPE;
    EEPROM_TC::instance()->setTempSetType(tempSetType);
  }
  switch (tempSetType) {
    case RAMP_TYPE:
      rampTimeEnd = EEPROM_TC::instance()->getTempRampTimeEnd();
      if (rampTimeEnd == 0xFFFFFFFF || rampTimeEnd == 0) {
        rampTimeEnd = 0;
        EEPROM_TC::instance()->setTempRampTimeEnd(rampTimeEnd);
        rampTimeStart = 0;
        EEPROM_TC::instance()->setTempRampTimeStart(rampTimeStart);
      } else {
        rampTimeStart = EEPROM_TC::instance()->getTempRampTimeStart();
        rampStartingTemp = EEPROM_TC::instance()->getRampStartingTemp();
      }
      break;
    case SINE_TYPE:
      period = EEPROM_TC::instance()->getTempSinePeriod();
      amplitude = EEPROM_TC::instance()->getTempSineAmplitude();
      sineStartTime = EEPROM_TC::instance()->getTempSineStartTime();
      break;
    default:
      break;
  }
  pinMode(TEMP_CONTROL_PIN, OUTPUT);
  digitalWrite(TEMP_CONTROL_PIN, TURN_SOLENOID_OFF);
  char buffer[10];
  dtostrf(targetTemperature, 5, 2, buffer);
  serial(F("%s starts with solenoid off with target temperature of %s C"),
         (this->isHeater() ? F("Heater") : F("Chiller")), buffer);
}

void TemperatureControl::setRampDuration(float newTempRampDuration) {
  if (newTempRampDuration > 0) {
    float currentRampTime = rampTimeEnd - rampTimeStart;
    char buffer1[10];
    char buffer2[10];
    dtostrf(currentRampTime, 5, 3, buffer1);
    dtostrf(newTempRampDuration, 5, 3, buffer2);
    serial(F("change ramp time from %s to %s"), buffer1, buffer2);
    rampTimeStart = DateTime_TC::now().secondstime();
    rampTimeEnd = rampTimeStart + (uint32_t)(newTempRampDuration * 3600);
    rampStartingTemp = TempProbe_TC::instance()->getRunningAverage();
    tempSetType = tempSetTypeTypes::RAMP_TYPE;
    EEPROM_TC::instance()->setTempSetType(tempSetType);
    EEPROM_TC::instance()->setTempRampTimeStart(rampTimeStart);
    EEPROM_TC::instance()->setTempRampTimeEnd(rampTimeEnd);
    EEPROM_TC::instance()->setRampStartingTemp(rampStartingTemp);
  } else {
    rampTimeEnd = 0;
    tempSetType = tempSetTypeTypes::FLAT_TYPE;
    EEPROM_TC::instance()->setTempSetType(tempSetType);
    EEPROM_TC::instance()->setTempRampTimeEnd(rampTimeEnd);
    serial("set ramp time to 0");
  }
}

void TemperatureControl::setSine(float sineAmplitude, float sinePeriodInHours) {
  period = (sinePeriodInHours * 3600);
  amplitude = sineAmplitude;
  tempSetType = tempSetTypeTypes::SINE_TYPE;
  sineStartTime = DateTime_TC::now().secondstime();
  EEPROM_TC::instance()->setTempSetType(tempSetType);
  EEPROM_TC::instance()->setTempSinePeriod(period);
  EEPROM_TC::instance()->setTempSineAmplitude(amplitude);
  EEPROM_TC::instance()->setTempSineStartTime(sineStartTime);
}

/**
 * is heater
 */
bool TemperatureControl::isHeater() {
  return true;
}

bool TemperatureControl::isOn() {
  return digitalRead(TEMP_CONTROL_PIN) == TURN_SOLENOID_ON;
}

/**
 * set target temperature and save in EEPROM
 */
void TemperatureControl::setTargetTemperature(float newTemperature) {
  if (targetTemperature != newTemperature) {
    char buffer1[10];
    char buffer2[10];
    dtostrf(targetTemperature, 5, 2, buffer1);
    dtostrf(newTemperature, 5, 2, buffer2);
    serial(F("change target temperature from %s to %s"), buffer1, buffer2);
    EEPROM_TC::instance()->setTemp(newTemperature);
    targetTemperature = newTemperature;
  }
}

void Chiller::updateControl(float currentTemperature) {
  uint32_t currentMillis = millis();
  uint32_t currentTime = DateTime_TC::now().secondstime();
  switch (tempSetType) {
    case FLAT_TYPE: {
      currentTemperatureTarget = targetTemperature;
      break;
    }
    case RAMP_TYPE: {
      if (currentTime < rampTimeEnd) {
        currentTemperatureTarget =
            rampStartingTemp +
            ((currentTime - rampTimeStart) * (targetTemperature - rampStartingTemp) / (rampTimeEnd - rampTimeStart));
      } else {
        currentTemperatureTarget = targetTemperature;
      }
      break;
    }
    case SINE_TYPE: {
      uint32_t sineEndTime = sineStartTime + period;
      if (currentTime >= sineEndTime) {
        sineStartTime = DateTime_TC::now().secondstime();
        sineEndTime = sineStartTime + period;
        EEPROM_TC::instance()->setTempSineStartTime(sineStartTime);
      }
      float timeLeftTillPeriodEnd = sineEndTime - currentTime;
      float percentNOTThroughPeriod = timeLeftTillPeriodEnd / period;
      float percentThroughPeriod = 1 - percentNOTThroughPeriod;
      float x = percentThroughPeriod * (2 * PI);                          // the x position for our sine wave
      currentTemperatureTarget = amplitude * sin(x) + targetTemperature;  // y position in our sine wave
      break;
    }
    default:
      break;
  }
  COUT("Chiller::updateControl(" << currentTemperature << ") at " << currentMillis);
  if (currentMillis < previousMillis) {
    COUT("Reset previousMillis from " << previousMillis << " to 0");
    previousMillis = 0;  // reset if clock went backwards (typical during tests)
  }
  // pause 30 seconds between switching chiller on and off to prevent damage to chiller
  if (currentMillis - previousMillis < TIME_INTERVAL) {
    COUT("Chiller update at " << currentMillis << " ignored due to update at " << previousMillis);
  } else {
    bool oldValue = digitalRead(TEMP_CONTROL_PIN);
    bool newValue;
    previousMillis = currentMillis;
    // if in calibration, turn unit off
    if (TankController::instance()->isInCalibration()) {
      newValue = TURN_SOLENOID_OFF;
      COUT("Chiller should be off");
    }
    // if the observed temperature is above the set-point range turn on the chiller
    else if (currentTemperature >= currentTemperatureTarget + DELTA) {
      newValue = TURN_SOLENOID_ON;
      COUT("Chiller should be on");
    }
    // if the observed temperature is below the set-point range turn off the chiller
    else if (currentTemperature <= currentTemperatureTarget - DELTA) {
      newValue = TURN_SOLENOID_OFF;
      COUT("Chiller should be off");
    } else {
      newValue = oldValue;
    }
    if (newValue != oldValue) {
      uint32_t currentMS = millis();
      serial(F("chiller turned %s at %lu after %lu ms"), newValue ? "off" : "on", currentMS, currentMS - lastSwitchMS);
      lastSwitchMS = currentMS;
      digitalWrite(TEMP_CONTROL_PIN, newValue);
    }
  }
}

void Heater::updateControl(float currentTemperature) {
  uint32_t currentTime = DateTime_TC::now().secondstime();
  switch (tempSetType) {
    case FLAT_TYPE: {
      currentTemperatureTarget = targetTemperature;
      break;
    }
    case RAMP_TYPE: {
      if (currentTime < rampTimeEnd) {
        currentTemperatureTarget =
            rampStartingTemp +
            ((currentTime - rampTimeStart) * (targetTemperature - rampStartingTemp) / (rampTimeEnd - rampTimeStart));
      } else {
        currentTemperatureTarget = targetTemperature;
      }
      break;
    }
    case SINE_TYPE: {
      uint32_t sineEndTime = sineStartTime + period;
      if (currentTime >= sineEndTime) {
        sineStartTime = DateTime_TC::now().secondstime();
        sineEndTime = sineStartTime + period;
        EEPROM_TC::instance()->setTempSineStartTime(sineStartTime);
      }
      float timeLeftTillPeriodEnd = sineEndTime - currentTime;
      float percentNOTThroughPeriod = timeLeftTillPeriodEnd / period;
      float percentThroughPeriod = 1 - percentNOTThroughPeriod;
      float x = percentThroughPeriod * (2 * PI);                          // the x position for our sine wave
      currentTemperatureTarget = amplitude * sin(x) + targetTemperature;  // y position in our sine wave
      break;
    }
    default:
      break;
  }
  COUT("Heater::updateControl(" << currentTemperature);
  bool oldValue = digitalRead(TEMP_CONTROL_PIN);
  bool newValue;
  // if in calibration, turn unit off
  if (TankController::instance()->isInCalibration()) {
    newValue = TURN_SOLENOID_OFF;
  }
  // if the observed temperature is below the temperature set-point range turn on the heater
  else if (currentTemperature <= currentTemperatureTarget - DELTA) {
    newValue = TURN_SOLENOID_ON;
  }
  // if the observed temperature is above the temperature set-point range turn off the heater
  else if (currentTemperature >= currentTemperatureTarget + DELTA) {
    newValue = TURN_SOLENOID_OFF;
  } else {
    newValue = oldValue;
    COUT("Heater update at " << millis() << " ignored due to recent change in state");
  }
  if (newValue != oldValue) {
    uint32_t currentMS = millis();
    serial(F("heater turned %s at %lu after %lu ms"), newValue ? "off" : "on", currentMS, currentMS - lastSwitchMS);
    lastSwitchMS = currentMS;
    digitalWrite(TEMP_CONTROL_PIN, newValue);
  }
}
