// ************************************************
// Load parameters from EEPROM
// ************************************************
void LoadParameters() {
  // Load from EEPROM
  phset = EepromReadDouble(PH_ADDRESS);
  tempset = EepromReadDouble(TEMP_ADDRESS);
  Kp = EepromReadDouble(KP_ADDRESS);
  Ki = EepromReadDouble(KI_ADDRESS);
  Kd = EepromReadDouble(KD_ADDRESS);
  heat = EepromReadDouble(HEAT_ADDRESS);
  amplitude = EepromReadDouble(AMPLITUDE_ADDRESS);
  frequency = EepromReadDouble(FREQUENCY_ADDRESS);
  // Use defaults if EEPROM values are invalid
  if (isnan(phset)) {
    phset = 8.1;
  }
  if (isnan(tempset)) {
    tempset = 20;
  }
  if (isnan(Kp)) {
    Kp = 100000;
  }
  if (isnan(Ki)) {
    Ki = 0;
  }
  if (isnan(Kd)) {
    Kd = 0;
  }
  if (isnan(heat)) {
    heat = 0;
  }
  if (isnan(amplitude)) {
    amplitude = 0;
  }
  if (isnan(frequency)) {
    frequency = 0;
  }
}