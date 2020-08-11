void SaveAmplitudeSet() {
  if (amplitudeSet != EEPROM_readDouble(AMPLITUDE_ADDRESS)) {
    EEPROM_writeDouble(AMPLITUDE_ADDRESS, amplitudeSet);
  }
}