void SaveAmplitudeSet() {
  if (amplitudeSet != EepromReadDouble(AMPLITUDE_ADDRESS)) {
    EepromWriteDouble(AMPLITUDE_ADDRESS, amplitudeSet);
  }
}