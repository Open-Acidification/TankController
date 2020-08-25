void SaveTempSet() {
  if (tempset != EEPROM_ReadDouble(TEMP_ADDRESS)) {
    EEPROM_WriteDouble(TEMP_ADDRESS, tempset);
  }
}