void SaveTempSet() {
  if (tempset != EepromReadDouble(TEMP_ADDRESS)) {
    EepromWriteDouble(TEMP_ADDRESS, tempset);
  }
}
