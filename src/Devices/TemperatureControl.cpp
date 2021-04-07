#include "TemperatureControl.h"

void Chiller::updateControl(double currentTemperature) {
  unsigned long currentMillis = millis();
  // pause 30 seconds between switching chiller on and off to prevent damage to chiller
  if (currentMillis - previousMillis >= TIME_INTERVAL) {
    previousMillis = currentMillis;
    // if the observed temperature is above the set point turn on the chiller
    if (currentTemperature >= targetTemperature + DELTA) {
      Serial.println(F("chiller on"));
      digitalWrite(PIN, LOW);
    }
    // if the observed temperature is below the set point turn off the chiller
    else if (currentTemperature <= targetTemperature - DELTA) {
      Serial.println(F("chiller off"));
      digitalWrite(PIN, HIGH);
    }
  }
}

void Heater::updateControl(double currentTemperature) {
  // if the observed temperature is below the temperature set-point turn on the heater
  if (currentTemperature <= targetTemperature - DELTA) {
    Serial.println(F("heater on"));
    digitalWrite(PIN, LOW);
  }
  // if the observed temperature is above the temperature set-point turn off the heater
  else if (currentTemperature >= targetTemperature + DELTA) {
    Serial.println(F("heater off"));
    digitalWrite(PIN, HIGH);
  }
}
