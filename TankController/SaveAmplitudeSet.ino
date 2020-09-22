void SaveAmplitudeSet() {
  if (amplitude_set != EepromReadDouble(AMPLITUDE_ADDRESS)) {
    EepromWriteDouble(AMPLITUDE_ADDRESS, amplitude_set);
  }
}
