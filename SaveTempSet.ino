void SaveTempSet()
{
  if (tempset != EEPROM_readDouble(tempAddress))
  {
    EEPROM_writeDouble(tempAddress, tempset);
  }
}