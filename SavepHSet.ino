// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SavepHSet()
{
  if (phset != EEPROM_readDouble(pHAddress))
  {
    EEPROM_writeDouble(pHAddress, phset);
  }
}