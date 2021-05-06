#include "TemperatureControl.h"

#include "Devices/EEPROM_TC.h"
#include "Serial_TC.h"
#include "TankControllerLib.h"

const double DEFAULT_TEMPERATURE = 20.0;

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

/**
 * static member function to return singleton
 */
void TemperatureControl::enableHeater(bool flag) {
  EEPROM_TC::instance()->setHeat(flag);
  if (_instance && (_instance->isHeater() != flag)) {
    delete _instance;
    _instance = nullptr;
    serial("TemperatureControl::enableHeater(%s)", flag ? "true" : "false");
    instance();
  }
}

/**
 * protected constructor
 */
TemperatureControl::TemperatureControl() {
  targetTemperature = EEPROM_TC::instance()->getTemp();
  if (isnan(targetTemperature)) {
    targetTemperature = DEFAULT_TEMPERATURE;
    EEPROM_TC::instance()->setTemp(targetTemperature);
  }
  digitalWrite(PIN, HIGH);
  serial("TemperatureControl::TemperatureControl() constructing %s with target Temperature of %f",
         this->isHeater() ? "Heater" : "Chiller", targetTemperature);
}

/**
 * is heater
 */
bool TemperatureControl::isHeater() {
  return true;
}

/**
 * set target temperature and save in EEPROM
 */
void TemperatureControl::setTargetTemperature(double newTemperature) {
  if (targetTemperature != newTemperature) {
    serial("Change target temperature from %6.3f to %6.3f", targetTemperature, newTemperature);
    EEPROM_TC::instance()->setTemp(newTemperature);
    targetTemperature = newTemperature;
  }
}

void Chiller::updateControl(double currentTemperature) {
  unsigned long currentMillis = millis();
  // pause 30 seconds between switching chiller on and off to prevent damage to chiller
  if (currentMillis - previousMillis >= TIME_INTERVAL) {
    bool oldValue = digitalRead(PIN);
    bool newValue = oldValue;
    previousMillis = currentMillis;
    // if in calibration, turn unit off
    if (TankControllerLib::instance()->isInCalibration()) {
      newValue = HIGH;
    }
    // if the observed temperature is above the set point turn on the chiller
    else if (currentTemperature >= targetTemperature + DELTA) {
      newValue = LOW;
    }
    // if the observed temperature is below the set point turn off the chiller
    else if (currentTemperature <= targetTemperature - DELTA) {
      newValue = HIGH;
    }
    if (newValue != oldValue) {
      serialWithTime((newValue ? "chiller off" : "chiller on"));
      digitalWrite(PIN, newValue);
    }
  }
}

void Heater::updateControl(double currentTemperature) {
  bool oldValue = digitalRead(PIN);
  bool newValue = oldValue;
  // if in calibration, turn unit off
  if (TankControllerLib::instance()->isInCalibration()) {
    newValue = HIGH;
  }
  // if the observed temperature is below the temperature set-point turn on the heater
  else if (currentTemperature <= targetTemperature - DELTA) {
    newValue = LOW;
  }
  // if the observed temperature is above the temperature set-point turn off the heater
  else if (currentTemperature >= targetTemperature + DELTA) {
    newValue = HIGH;
  }
  if (newValue != oldValue) {
    serialWithTime((newValue ? "heater off" : "heater on"));
    digitalWrite(PIN, newValue);
  }
}
