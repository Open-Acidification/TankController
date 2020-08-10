void SaveTempSet() {
  if (tempset != EEPROM_readDouble(TEMP_ADDRESS)) {
    EEPROM_writeDouble(TEMP_ADDRESS, tempset);
  }
}