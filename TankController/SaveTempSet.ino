void SaveTempSet() {
  if (temp_set != EepromReadDouble(TEMP_ADDRESS)) {
    EepromWriteDouble(TEMP_ADDRESS, temp_set);
  }
}
