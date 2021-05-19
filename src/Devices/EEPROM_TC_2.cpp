#include "Devices/EEPROM_TC_2.h"

#include "TC_util.h"

//  instance methods
bool EEPROM_TC_2::isRightVersion() {
  return true;  // we have no way of telling!
}

// getter methods
float EEPROM_TC_2::getAmplitude() {
  return eepromReadFloat(AMPLITUDE_ADDRESS);
}
float EEPROM_TC_2::getCorrectedTemp() {
  return eepromReadFloat(TEMP_CORR_ADDRESS);
}
float EEPROM_TC_2::getFrequency() {
  return eepromReadFloat(FREQUENCY_ADDRESS);
}
int EEPROM_TC_2::getGoogleSheetInterval() {
  return eepromReadInt(GOOGLE_INTERVAL_ADDRESS);
}
float EEPROM_TC_2::getGranularity() {
  return eepromReadFloat(GRANULARITY_ADDRESS);
}
bool EEPROM_TC_2::getHeat() {
  return (bool)eepromReadFloat(HEAT_ADDRESS);
}
float EEPROM_TC_2::getKD() {
  return eepromReadFloat(KD_ADDRESS);
}
float EEPROM_TC_2::getKI() {
  return eepromReadFloat(KI_ADDRESS);
}
float EEPROM_TC_2::getKP() {
  return eepromReadFloat(KP_ADDRESS);
}
float EEPROM_TC_2::getMac() {
  return eepromReadFloat(MAC_ADDRESS);
}
float EEPROM_TC_2::getMaxDataAge() {
  return eepromReadFloat(MAX_DATA_AGE_ADDRESS);
}
float EEPROM_TC_2::getPH() {
  return eepromReadFloat(PH_ADDRESS);
}
float EEPROM_TC_2::getPHDelay() {
  return eepromReadFloat(PH_DELAY_ADDRESS);
}
float EEPROM_TC_2::getPHInterval() {
  return eepromReadFloat(PH_INTERVAL_ADDRESS);
}
float EEPROM_TC_2::getPHSeriesPointer() {
  return eepromReadFloat(PH_SERIES_POINTER_ADDRESS);
}
float EEPROM_TC_2::getPHSeriesSize() {
  return eepromReadFloat(PH_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC_2::getTankID() {
  return (int)eepromReadFloat(TANK_ID_ADDRESS);
}
float EEPROM_TC_2::getTemp() {
  return eepromReadFloat(TEMP_ADDRESS);
}
float EEPROM_TC_2::getTempDelay() {
  return eepromReadFloat(TEMP_DELAY_ADDRESS);
}
float EEPROM_TC_2::getTempInterval() {
  return eepromReadFloat(TEMP_INTERVAL_ADDRESS);
}
float EEPROM_TC_2::getTempSeriesPointer() {
  return eepromReadFloat(TEMP_SERIES_POINTER_ADDRESS);
}
float EEPROM_TC_2::getTempSeriesSize() {
  return eepromReadFloat(TEMP_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC_2::getVersion() {
  return 2;  // we assume so!
}

// setter methods
void EEPROM_TC_2::setAmplitude(float value) {
  eepromWriteFloat(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC_2::setCorrectedTemp(float value) {
  eepromWriteFloat(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC_2::setFrequency(float value) {
  eepromWriteFloat(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC_2::setGoogleSheetInterval(int value) {
  eepromWriteInt(GOOGLE_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_2::setGranularity(float value) {
  eepromWriteFloat(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC_2::setHeat(bool value) {
  eepromWriteFloat(HEAT_ADDRESS, (float)value);
}
void EEPROM_TC_2::setKD(float value) {
  eepromWriteFloat(KD_ADDRESS, value);
}
void EEPROM_TC_2::setKI(float value) {
  eepromWriteFloat(KI_ADDRESS, value);
}
void EEPROM_TC_2::setKP(float value) {
  eepromWriteFloat(KP_ADDRESS, value);
}
void EEPROM_TC_2::setMac(float value) {
  eepromWriteFloat(MAC_ADDRESS, value);
}
void EEPROM_TC_2::setMaxDataAge(float value) {
  eepromWriteFloat(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC_2::setPH(float value) {
  eepromWriteFloat(PH_ADDRESS, value);
}
void EEPROM_TC_2::setPHDelay(float value) {
  eepromWriteFloat(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC_2::setPHInterval(float value) {
  eepromWriteFloat(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_2::setPHSeriesPointer(float value) {
  eepromWriteFloat(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_2::setPHSeriesSize(float value) {
  eepromWriteFloat(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC_2::setTankID(int value) {
  eepromWriteFloat(TANK_ID_ADDRESS, (float)value);
}
void EEPROM_TC_2::setTemp(float value) {
  eepromWriteFloat(TEMP_ADDRESS, value);
}
void EEPROM_TC_2::setTempDelay(float value) {
  eepromWriteFloat(TEMP_DELAY_ADDRESS, value);
}
void EEPROM_TC_2::setTempInterval(float value) {
  eepromWriteFloat(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_2::setTempSeriesPointer(float value) {
  eepromWriteFloat(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_2::setTempSeriesSize(float value) {
  eepromWriteFloat(TEMP_SERIES_SIZE_ADDRESS, value);
}
