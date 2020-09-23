// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput() {
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if (now - window_start_time > WINDOW_SIZE) {  // time to shift the Relay Window
    window_start_time += WINDOW_SIZE;
  }
  if ((on_time > 100) && (on_time > (now - window_start_time))) {
    digitalWrite(CO2_REG, LOW);  // OPEN CO2 solenoid
  } else {
    digitalWrite(CO2_REG, HIGH);  // CLOSE CO2 solenoid
  }
}