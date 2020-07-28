void SaveAmplitudeSet() {
  if (amplitudeSet != EEPROM_readDouble(amplitudeAddress)) {
    EEPROM_writeDouble(amplitudeAddress, amplitudeSet);
  }
}