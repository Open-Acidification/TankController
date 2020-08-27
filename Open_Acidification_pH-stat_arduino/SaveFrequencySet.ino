void SaveFrequencySet() {
  if (frequencySet != EepromReadDouble(FREQUENCY_ADDRESS)) {
    EepromWriteDouble(FREQUENCY_ADDRESS, frequencySet);
  }
}