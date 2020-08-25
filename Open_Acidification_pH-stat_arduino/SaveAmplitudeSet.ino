void SaveAmplitudeSet() {
  if (amplitudeSet != EEPROM_ReadDouble(AMPLITUDE_ADDRESS)) {
    EEPROM_WriteDouble(AMPLITUDE_ADDRESS, amplitudeSet);
  }
}