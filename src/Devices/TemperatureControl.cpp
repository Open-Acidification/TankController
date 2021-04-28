#include "TemperatureControl.h"

#include "Devices/EEPROM_TC.h"
#include "Serial_TC.h"

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
  }
}

/**
 * private constructor
 */
TemperatureControl::TemperatureControl() {
  targetTemperature = EEPROM_TC::instance()->getTemp();
  digitalWrite(PIN, HIGH);
}

/**
 * set target temperature and save in EEPROM
 */
void TemperatureControl::setTargetTemperature(double newTemperature) {
  EEPROM_TC::instance()->setTemp(newTemperature);
  targetTemperature = newTemperature;
}

void Chiller::updateControl(double currentTemperature) {
  unsigned long currentMillis = millis();
  // pause 30 seconds between switching chiller on and off to prevent damage to chiller
  if (currentMillis - previousMillis >= TIME_INTERVAL) {
    bool oldValue = digitalRead(PIN);
    bool newValue = oldValue;
    previousMillis = currentMillis;
    // if the observed temperature is above the set point turn on the chiller
    if (currentTemperature >= targetTemperature + DELTA) {
      newValue = LOW;
    }
    // if the observed temperature is below the set point turn off the chiller
    else if (currentTemperature <= targetTemperature - DELTA) {
      newValue = HIGH;
    }
    if (newValue != oldValue) {
      Serial_TC::instance()->ts_printf((const char *)(newValue ? F("chiller off") : F("chiller on")));
      digitalWrite(PIN, newValue);
    }
  }
}

void Heater::updateControl(double currentTemperature) {
  bool oldValue = digitalRead(PIN);
  bool newValue = oldValue;
  // if the observed temperature is below the temperature set-point turn on the heater
  if (currentTemperature <= targetTemperature - DELTA) {
    newValue = LOW;
  }
  // if the observed temperature is above the temperature set-point turn off the heater
  else if (currentTemperature >= targetTemperature + DELTA) {
    newValue = HIGH;
  }
  if (newValue != oldValue) {
    Serial_TC::instance()->ts_printf((const char *)(newValue ? F("heater off") : F("heater on")));
    digitalWrite(PIN, newValue);
  }
}
