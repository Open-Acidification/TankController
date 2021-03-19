#include "Devices/EEPROM_TC_3.h"

#include "TC_assert.h"

//  instance methods
bool EEPROM_TC_3::isRightVersion() {
  int version = getVersion();
  // the initial value in EEPROM is -1, so we are starting clean
  return version == -1 || version == 3;
}

// getter methods
double EEPROM_TC_3::getAmplitude() {
  return eepromReadDouble(AMPLITUDE_ADDRESS);
}
double EEPROM_TC_3::getCorrectedTemp() {
  return eepromReadDouble(TEMP_CORR_ADDRESS);
}
double EEPROM_TC_3::getFrequency() {
  return eepromReadDouble(FREQUENCY_ADDRESS);
}
int EEPROM_TC_3::getGoogleSheetInterval() {
  return eepromReadInt(GOOGLE_INTERVAL_ADDRESS);
}
double EEPROM_TC_3::getGranularity() {
  return eepromReadDouble(GRANULARITY_ADDRESS);
}
bool EEPROM_TC_3::getHeat() {
  return (bool)eepromReadInt(HEAT_ADDRESS);
}
double EEPROM_TC_3::getKD() {
  return eepromReadDouble(KD_ADDRESS);
}
double EEPROM_TC_3::getKI() {
  return eepromReadDouble(KI_ADDRESS);
}
double EEPROM_TC_3::getKP() {
  return eepromReadDouble(KP_ADDRESS);
}
double EEPROM_TC_3::getMac() {
  return eepromReadDouble(MAC_ADDRESS);
}
double EEPROM_TC_3::getMaxDataAge() {
  return eepromReadDouble(MAX_DATA_AGE_ADDRESS);
}
double EEPROM_TC_3::getPH() {
  return eepromReadDouble(PH_ADDRESS);
}
double EEPROM_TC_3::getPHDelay() {
  return eepromReadDouble(PH_DELAY_ADDRESS);
}
double EEPROM_TC_3::getPHInterval() {
  return eepromReadDouble(PH_INTERVAL_ADDRESS);
}
double EEPROM_TC_3::getPHSeriesPointer() {
  return eepromReadDouble(PH_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC_3::getPHSeriesSize() {
  return eepromReadDouble(PH_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC_3::getTankID() {
  return eepromReadInt(TANK_ID_ADDRESS);
}
double EEPROM_TC_3::getTemp() {
  return eepromReadDouble(TEMP_ADDRESS);
}
double EEPROM_TC_3::getTempDelay() {
  return eepromReadDouble(TEMP_DELAY_ADDRESS);
}
double EEPROM_TC_3::getTempInterval() {
  return eepromReadDouble(TEMP_INTERVAL_ADDRESS);
}
double EEPROM_TC_3::getTempSeriesPointer() {
  return eepromReadDouble(TEMP_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC_3::getTempSeriesSize() {
  return eepromReadDouble(TEMP_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC_3::getVersion() {
  int result = eepromReadInt(VERSION_ADDRESS);
  if (result == -1) {
    // the initial value in EEPROM is -1, so we are starting clean
    setVersion();
    result = eepromReadInt(VERSION_ADDRESS);
  }
  return result;
}

// setter methods
void EEPROM_TC_3::setAmplitude(double value) {
  eepromWriteDouble(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC_3::setCorrectedTemp(double value) {
  eepromWriteDouble(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC_3::setFrequency(double value) {
  eepromWriteDouble(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC_3::setGoogleSheetInterval(int value) {
  eepromWriteInt(GOOGLE_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_3::setGranularity(double value) {
  eepromWriteDouble(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC_3::setHeat(bool value) {
  eepromWriteInt(HEAT_ADDRESS, (int)value);
}
void EEPROM_TC_3::setKD(double value) {
  eepromWriteDouble(KD_ADDRESS, value);
}
void EEPROM_TC_3::setKI(double value) {
  eepromWriteDouble(KI_ADDRESS, value);
}
void EEPROM_TC_3::setKP(double value) {
  eepromWriteDouble(KP_ADDRESS, value);
}
void EEPROM_TC_3::setMac(double value) {
  eepromWriteDouble(MAC_ADDRESS, value);
}
void EEPROM_TC_3::setMaxDataAge(double value) {
  eepromWriteDouble(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC_3::setPH(double value) {
  eepromWriteDouble(PH_ADDRESS, value);
}
void EEPROM_TC_3::setPHDelay(double value) {
  eepromWriteDouble(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC_3::setPHInterval(double value) {
  eepromWriteDouble(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_3::setPHSeriesPointer(double value) {
  eepromWriteDouble(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_3::setPHSeriesSize(double value) {
  eepromWriteDouble(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC_3::setTankID(int value) {
  eepromWriteInt(TANK_ID_ADDRESS, value);
}
void EEPROM_TC_3::setTemp(double value) {
  eepromWriteDouble(TEMP_ADDRESS, value);
}
void EEPROM_TC_3::setTempDelay(double value) {
  eepromWriteDouble(TEMP_DELAY_ADDRESS, value);
}
void EEPROM_TC_3::setTempInterval(double value) {
  eepromWriteDouble(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_3::setTempSeriesPointer(double value) {
  eepromWriteDouble(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_3::setTempSeriesSize(double value) {
  eepromWriteDouble(TEMP_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC_3::setVersion() {
  eepromWriteInt(VERSION_ADDRESS, 3);
}
