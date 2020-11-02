

class TC_Chiller {
private:
  int pin = 47;
  unsigned long currentTime = millis();
  unsigned long previousTime = 0;

public:
  TC_Chiller() {
  }

  TC_Chiller(int pinNum) {
    pin = pinNum;
  }

  void chill() {
    if (currentTime - previousTime >= CHILLER_INTERVAL) {
      previousTime = currentTime;
      if (temp >= temp_set + 0.05) {
        Serial.println(F("chiller on"));
        digitalWrite(CHILLER, LOW);
      } else if (temp <= temp_set - 0.05) {
        Serial.println(F("chiller off"));
        digitalWrite(CHILLER, HIGH);
      }
    }
  }

  void noChill() {
    if (temp <= temp_set + 0.05) {      // if the observed temperature is less than or equal the temperature setpoint plus .05 degree
      Serial.println(F("chiller on"));  // print chiller state to serial
      digitalWrite(CHILLER, LOW);
    } else if (temp >= temp_set - 0.05) {  // see if temperature is greater than or equal to .05 below setpoint
      Serial.println(F("chiller off"));    // print chiller state to serial
      digitalWrite(CHILLER, HIGH);
    }
  }

  void setChiller() {
    if (heat == 0) {
      chill();
    } else if (heat == 1) {
      noChill();
    }
  }
};
