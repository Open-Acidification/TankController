void SaveFrequencySet() {
  if (frequencySet != EEPROM_readDouble(frequencyAddress)) {
    EEPROM_writeDouble(frequencyAddress, frequencySet);
  }
}