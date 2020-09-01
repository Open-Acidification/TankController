void SaveFrequencySet() {
  if (frequency_set != EepromReadDouble(FREQUENCY_ADDRESS)) {
    EepromWriteDouble(FREQUENCY_ADDRESS, frequency_set);
  }
}
