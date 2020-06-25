// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SavepHSet() {
  if (phset != EEPROM_readDouble(pHAddress)) {
    EEPROM_writeDouble(pHAddress, phset);
  }
}
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
// ************************************************
// Read floating point values from EEPROM
// ************************************************
double EEPROM_readDouble(int address) {
  double value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}
void SaveFrequencySet() {
  if (frequencySet != EEPROM_readDouble(frequencyAddress)) {
    EEPROM_writeDouble(frequencyAddress, frequencySet);
  }
}
// ************************************************
// Write floating point values to EEPROM
// ************************************************
void EEPROM_writeDouble(int address, double value) {
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    EEPROM.write(address++, *p++);
  }
}
void SaveAmplitudeSet() {
  if (amplitudeSet != EEPROM_readDouble(amplitudeAddress)) {
    EEPROM_writeDouble(amplitudeAddress, amplitudeSet);
  }
}
void SaveTempSet() {
  if (tempset != EEPROM_readDouble(tempAddress)) {
    EEPROM_writeDouble(tempAddress, tempset);
  }
}
