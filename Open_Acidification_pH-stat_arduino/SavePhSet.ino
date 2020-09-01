// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SavePhSet() {
  if (ph_set != EepromReadDouble(PH_ADDRESS)) {
    EepromWriteDouble(PH_ADDRESS, ph_set);
  }
}
