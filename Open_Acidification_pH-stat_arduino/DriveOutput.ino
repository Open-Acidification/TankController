// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput() {
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if (now - windowStartTime > WINDOW_SIZE) {  // time to shift the Relay Window
    windowStartTime += WINDOW_SIZE;
  }
  if ((onTime > 100) && (onTime > (now - windowStartTime))) {
    digitalWrite(CO2_REG, LOW);  // OPEN CO2 solenoid
  } else {
    digitalWrite(CO2_REG, HIGH);  // CLOSE CO2 solenoid
  }
}