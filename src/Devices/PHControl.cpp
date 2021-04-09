#include "PHControl.h"
#include "PID_TC.h"

PHControl::PHControl() {
  window_start_time = millis();
  digitalWrite(PIN, HIGH);
}

void PHControl::updateControl(double pH) {
  if (usePID) {
    onTime = PID_TC::instance()->computeOutput(targetPh, pH);
  } else {
    onTime = pH > targetPh ? 10000 : 0;
  }
  long now = millis();
  if (now - window_start_time > WINDOW_SIZE) {  // time to shift the Relay Window
    window_start_time += WINDOW_SIZE;
  }
  if ((onTime > SOLENOID_OPENING_TIME) && (onTime > (now - window_start_time))) {
    digitalWrite(PIN, LOW);  // OPEN CO2 solenoid
  } else {
    digitalWrite(PIN, HIGH);  // CLOSE CO2 solenoid
  }
}
