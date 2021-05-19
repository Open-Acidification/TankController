#include "Devices/EEPROM_TC_3.h"

#include "TC_util.h"

//  instance methods
bool EEPROM_TC_3::isRightVersion() {
  int version = getVersion();
  // the initial value in EEPROM is -1, so we are starting clean
  return version == -1 || version == 3;
}

// getter methods
float EEPROM_TC_3::getAmplitude() {
  return eepromReadFloat(AMPLITUDE_ADDRESS);
}
float EEPROM_TC_3::getCorrectedTemp() {
  return eepromReadFloat(TEMP_CORR_ADDRESS);
}
float EEPROM_TC_3::getFrequency() {
  return eepromReadFloat(FREQUENCY_ADDRESS);
}
int EEPROM_TC_3::getGoogleSheetInterval() {
  return eepromReadInt(GOOGLE_INTERVAL_ADDRESS);
}
float EEPROM_TC_3::getGranularity() {
  return eepromReadFloat(GRANULARITY_ADDRESS);
}
bool EEPROM_TC_3::getHeat() {
  return (bool)eepromReadInt(HEAT_ADDRESS);
}
float EEPROM_TC_3::getKD() {
  return eepromReadFloat(KD_ADDRESS);
}
float EEPROM_TC_3::getKI() {
  return eepromReadFloat(KI_ADDRESS);
}
float EEPROM_TC_3::getKP() {
  return eepromReadFloat(KP_ADDRESS);
}
float EEPROM_TC_3::getMac() {
  return eepromReadFloat(MAC_ADDRESS);
}
float EEPROM_TC_3::getMaxDataAge() {
  return eepromReadFloat(MAX_DATA_AGE_ADDRESS);
}
float EEPROM_TC_3::getPH() {
  return eepromReadFloat(PH_ADDRESS);
}
float EEPROM_TC_3::getPHDelay() {
  return eepromReadFloat(PH_DELAY_ADDRESS);
}
float EEPROM_TC_3::getPHInterval() {
  return eepromReadFloat(PH_INTERVAL_ADDRESS);
}
float EEPROM_TC_3::getPHSeriesPointer() {
  return eepromReadFloat(PH_SERIES_POINTER_ADDRESS);
}
float EEPROM_TC_3::getPHSeriesSize() {
  return eepromReadFloat(PH_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC_3::getTankID() {
  return eepromReadInt(TANK_ID_ADDRESS);
}
float EEPROM_TC_3::getTemp() {
  return eepromReadFloat(TEMP_ADDRESS);
}
float EEPROM_TC_3::getTempDelay() {
  return eepromReadFloat(TEMP_DELAY_ADDRESS);
}
float EEPROM_TC_3::getTempInterval() {
  return eepromReadFloat(TEMP_INTERVAL_ADDRESS);
}
float EEPROM_TC_3::getTempSeriesPointer() {
  return eepromReadFloat(TEMP_SERIES_POINTER_ADDRESS);
}
float EEPROM_TC_3::getTempSeriesSize() {
  return eepromReadFloat(TEMP_SERIES_SIZE_ADDRESS);
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
void EEPROM_TC_3::setAmplitude(float value) {
  eepromWriteFloat(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC_3::setCorrectedTemp(float value) {
  eepromWriteFloat(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC_3::setFrequency(float value) {
  eepromWriteFloat(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC_3::setGoogleSheetInterval(int value) {
  eepromWriteInt(GOOGLE_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_3::setGranularity(float value) {
  eepromWriteFloat(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC_3::setHeat(bool value) {
  eepromWriteInt(HEAT_ADDRESS, (int)value);
}
void EEPROM_TC_3::setKD(float value) {
  eepromWriteFloat(KD_ADDRESS, value);
}
void EEPROM_TC_3::setKI(float value) {
  eepromWriteFloat(KI_ADDRESS, value);
}
void EEPROM_TC_3::setKP(float value) {
  eepromWriteFloat(KP_ADDRESS, value);
}
void EEPROM_TC_3::setMac(float value) {
  eepromWriteFloat(MAC_ADDRESS, value);
}
void EEPROM_TC_3::setMaxDataAge(float value) {
  eepromWriteFloat(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC_3::setPH(float value) {
  eepromWriteFloat(PH_ADDRESS, value);
}
void EEPROM_TC_3::setPHDelay(float value) {
  eepromWriteFloat(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC_3::setPHInterval(float value) {
  eepromWriteFloat(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_3::setPHSeriesPointer(float value) {
  eepromWriteFloat(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_3::setPHSeriesSize(float value) {
  eepromWriteFloat(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC_3::setTankID(int value) {
  eepromWriteInt(TANK_ID_ADDRESS, value);
}
void EEPROM_TC_3::setTemp(float value) {
  eepromWriteFloat(TEMP_ADDRESS, value);
}
void EEPROM_TC_3::setTempDelay(float value) {
  eepromWriteFloat(TEMP_DELAY_ADDRESS, value);
}
void EEPROM_TC_3::setTempInterval(float value) {
  eepromWriteFloat(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC_3::setTempSeriesPointer(float value) {
  eepromWriteFloat(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC_3::setTempSeriesSize(float value) {
  eepromWriteFloat(TEMP_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC_3::setVersion() {
  eepromWriteInt(VERSION_ADDRESS, 3);
}
