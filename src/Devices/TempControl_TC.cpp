#include "TempControl_TC.h"


void TempControl_TC::updateCtrl() {
  
}

void Chiller_TC::chill() {
  if (currentTime - previousTime >= CHILLER_INTERVAL) {
    previousTime = currentTime;
    // temp and temp_set may need to be passed in since they're dependent on GetTemperature
    if (temp >= temp_set + 0.05) {
      Serial.println(F("chiller on"));
      digitalWrite(CHILLER, LOW);
    } else if (temp <= temp_set - 0.05) {
      Serial.println(F("chiller off"));
      digitalWrite(CHILLER, HIGH);
    }
  }
}

void Heater_TC::heat() {
  if (temp <=
      temp_set + 0.05) {  // if the observed temperature is less than or equal the temperature setpoint plus .05 degree
    Serial.println(F("chiller on"));  // print chiller state to serial
    digitalWrite(CHILLER, LOW);
  } else if (temp >= temp_set - 0.05) {  // see if temperature is greater than or equal to .05 below setpoint
    Serial.println(F("chiller off"));    // print chiller state to serial
    digitalWrite(CHILLER, HIGH);
  }
}
