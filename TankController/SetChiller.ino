// ************************************************
// Set chiller state
// ************************************************

void SetChiller() {
  if (heat == 0) {
    unsigned long chiller_current_millis = millis();
    if (chiller_current_millis - chiller_previous_millis >= CHILLER_INTERVAL) {  // pause 30 seconds between swtiching chiller on and off to prevent damage to chiller
      chiller_previous_millis = chiller_current_millis;
      if (temp >= temp_set + 0.05) {      // if the observed temperature is greater than or equal the temperature setpoint plus .05 degree
        Serial.println(F("chiller on"));  // print chiller state to serial
        digitalWrite(CHILLER, LOW);
      }
      if (temp <= temp_set - 0.05) {       // see if temperature is lower than .05 below setpoint
        Serial.println(F("chiller off"));  // print chiller state to serial
        digitalWrite(CHILLER, HIGH);
      }
    }
  }
  if (heat == 1) {
    if (temp <= temp_set + 0.05) {      // if the observed temperature is less than or equal the temperature setpoint plus .05 degree
      Serial.println(F("chiller on"));  // print chiller state to serial
      digitalWrite(CHILLER, LOW);
    }
    if (temp >= temp_set - 0.05) {       // see if temperature is greater than or equal to .05 below setpoint
      Serial.println(F("chiller off"));  // print chiller state to serial
      digitalWrite(CHILLER, HIGH);
    }
  }
}