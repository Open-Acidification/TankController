// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SavepHSet() {
  if (phset != EEPROM_readDouble(PH_ADDRESS)) {
    EEPROM_writeDouble(PH_ADDRESS, phset);
  }
}