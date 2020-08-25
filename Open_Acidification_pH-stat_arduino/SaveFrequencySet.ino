void SaveFrequencySet() {
  if (frequencySet != EEPROM_ReadDouble(FREQUENCY_ADDRESS)) {
    EEPROM_WriteDouble(FREQUENCY_ADDRESS, frequencySet);
  }
}