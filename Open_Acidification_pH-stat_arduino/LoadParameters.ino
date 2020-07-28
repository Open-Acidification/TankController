// ************************************************
// Load parameters from EEPROM
// ************************************************
void LoadParameters() {
  // Load from EEPROM
  phset = EEPROM_readDouble(pHAddress);
  tempset = EEPROM_readDouble(tempAddress);
  Kp = EEPROM_readDouble(KpAddress);
  Ki = EEPROM_readDouble(KiAddress);
  Kd = EEPROM_readDouble(KdAddress);
  heat = EEPROM_readDouble(heatAddress);
  amplitude = EEPROM_readDouble(amplitudeAddress);
  frequency = EEPROM_readDouble(frequencyAddress);
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