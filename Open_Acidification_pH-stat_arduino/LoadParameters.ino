// ************************************************
// Load parameters from EEPROM
// ************************************************
void LoadParameters() {
  // Load from EEPROM
  phset = EEPROM_readDouble(PH_ADDRESS);
  tempset = EEPROM_readDouble(TEMP_ADDRESS);
  Kp = EEPROM_readDouble(KP_ADDRESS);
  Ki = EEPROM_readDouble(KI_ADDRESS);
  Kd = EEPROM_readDouble(KD_ADDRESS);
  heat = EEPROM_readDouble(HEAT_ADDRESS);
  amplitude = EEPROM_readDouble(AMPLITUDE_ADDRESS);
  frequency = EEPROM_readDouble(FREQUENCY_ADDRESS);
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