// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void Save_pH_Set() {
  if (phset != EEPROM_ReadDouble(PH_ADDRESS)) {
    EEPROM_WriteDouble(PH_ADDRESS, phset);
  }
}