#include "Devices/EEPROM_TC_2.h"

#include "TC_util.h"

//  instance methods
bool EEPROM_TC_2::isRightVersion() {
  return true;  // we have no way of telling!
}

// getter methods
double EEPROM_TC_2::getAmplitude() {
  return eepromReadDouble(AMPLITUDE_ADDRESS);
}
double EEPROM_TC_2::getCorrectedTemp() {
  return eepromReadDouble(TEMP_CORR_ADDRESS);
}
double EEPROM_TC_2::getFrequency() {
  return eepromReadDouble(FREQUENCY_ADDRESS);
}
int EEPROM_TC_2::getGoogleSheetInterval() {
  return eepromReadInt(GOOGLE_INTERVAL_ADDRESS);
}
double EEPROM_TC_2::getGranularity() {
  return eepromReadDouble(GRANULARITY_ADDRESS);
}
bool EEPROM_TC_2::getHeat() {
  return (bool)eepromReadDouble(HEAT_ADDRESS);
}
double EEPROM_TC_2::getKD() {
  return eepromReadDouble(KD_ADDRESS);
}
double EEPROM_TC_2::getKI() {
  return eepromReadDouble(KI_ADDRESS);
}
double EEPROM_TC_2::getKP() {
  return eepromReadDouble(KP_ADDRESS);
}
double EEPROM_TC_2::getMac() {
  return eepromReadDouble(MAC_ADDRESS);
}
double EEPROM_TC_2::getMaxDataAge() {
  return eepromReadDouble(MAX_DATA_AGE_ADDRESS);
}
double EEPROM_TC_2::getPH() {
  return eepromReadDouble(PH_ADDRESS);
}
double EEPROM_TC_2::getPHDelay() {
  return eepromReadDouble(PH_DELAY_ADDRESS);
}
double EEPROM_TC_2::getPHInterval() {
  return eepromReadDouble(PH_INTERVAL_ADDRESS);
}
double EEPROM_TC_2::getPHSeriesPointer() {
  return eepromReadDouble(PH_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC_2::getPHSeriesSize() {
  return eepromReadDouble(PH_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC_2::getTankID() {
  return (int)eepromReadDouble(TANK_ID_ADDRESS);
}
double EEPROM_TC_2::getTemp() {
  return eepromReadDouble(TEMP_ADDRESS);
}
double EEPROM_TC_2::getTempDelay() {
  return eepromReadDouble(TEMP_DELAY_ADDRESS);
}
double EEPROM_TC_2::getTempInterval() {
  return eepromReadDouble(TEMP_INTERVAL_ADDRESS);
}
double EEPROM_TC_2::getTempSeriesPointer() {
  return eepromReadDouble(TEMP_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC_2::getTempSeriesSize() {
  return eepromReadDouble(TEMP_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC_2::getVersion() {
  return 2;  // we assume so!
}

// setter methods
void EEPROM_TC_2::setAmplitude(double value) {
  eepromWriteDouble(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC_2::setCorrectedTemp(double value) {
  eepromWriteDouble(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC_2::setFrequency(double value) {
  eepromWriteDouble(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC_2::setGoogleSheetInterval(int value) {
  eepromWriteInt(GOOGLE_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_2::setGranularity(double value) {
  eepromWriteDouble(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC_2::setHeat(bool value) {
  eepromWriteDouble(HEAT_ADDRESS, (double)value);
}
void EEPROM_TC_2::setKD(double value) {
  eepromWriteDouble(KD_ADDRESS, value);
}
void EEPROM_TC_2::setKI(double value) {
  eepromWriteDouble(KI_ADDRESS, value);
}
void EEPROM_TC_2::setKP(double value) {
  eepromWriteDouble(KP_ADDRESS, value);
}
void EEPROM_TC_2::setMac(double value) {
  eepromWriteDouble(MAC_ADDRESS, value);
}
void EEPROM_TC_2::setMaxDataAge(double value) {
  eepromWriteDouble(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC_2::setPH(double value) {
  eepromWriteDouble(PH_ADDRESS, value);
}
void EEPROM_TC_2::setPHDelay(double value) {
  eepromWriteDouble(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC_2::setPHInterval(double value) {
  eepromWriteDouble(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_2::setPHSeriesPointer(double value) {
  eepromWriteDouble(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_2::setPHSeriesSize(double value) {
  eepromWriteDouble(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC_2::setTankID(int value) {
  eepromWriteDouble(TANK_ID_ADDRESS, (double)value);
}
void EEPROM_TC_2::setTemp(double value) {
  eepromWriteDouble(TEMP_ADDRESS, value);
}
void EEPROM_TC_2::setTempDelay(double value) {
  eepromWriteDouble(TEMP_DELAY_ADDRESS, value);
}
void EEPROM_TC_2::setTempInterval(double value) {
  eepromWriteDouble(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_2::setTempSeriesPointer(double value) {
  eepromWriteDouble(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_2::setTempSeriesSize(double value) {
  eepromWriteDouble(TEMP_SERIES_SIZE_ADDRESS, value);
}
