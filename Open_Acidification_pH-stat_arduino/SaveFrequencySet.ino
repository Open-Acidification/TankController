void SaveFrequencySet() {
  if (frequencySet != EEPROM_readDouble(FREQUENCY_ADDRESS)) {
    EEPROM_writeDouble(FREQUENCY_ADDRESS, frequencySet);
  }
}