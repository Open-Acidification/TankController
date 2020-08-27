// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SavePhSet() {
  if (phset != EepromReadDouble(PH_ADDRESS)) {
    EepromWriteDouble(PH_ADDRESS, phset);
  }
}
