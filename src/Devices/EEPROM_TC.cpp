#include "Devices/EEPROM_TC.h"

//  class variables
EEPROM_TC* EEPROM_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
EEPROM_TC* EEPROM_TC::instance() {
  if (!_instance) {
    _instance = new EEPROM_TC();
  }
  return _instance;
}

//  instance methods
EEPROM_TC::EEPROM_TC() {
}

double EEPROM_TC::eepromReadDouble(int address) {
  double value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

void EEPROM_TC::eepromWriteDouble(int address, double value) {
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    EEPROM.update(address++, *p++);
  }
}

// getter methods
double EEPROM_TC::getPH() {
  return eepromReadDouble(PH_ADDRESS);
}
double EEPROM_TC::getTemp() {
  return eepromReadDouble(TEMP_ADDRESS);
}
double EEPROM_TC::getTankID() {
  return eepromReadDouble(TANK_ID_ADDRESS);
}
double EEPROM_TC::getCorrectedTemp() {
  return eepromReadDouble(TEMP_CORR_ADDRESS);
}
double EEPROM_TC::getKP() {
  return eepromReadDouble(KP_ADDRESS);
}
double EEPROM_TC::getKI() {
  return eepromReadDouble(KI_ADDRESS);
}
double EEPROM_TC::getKD() {
  return eepromReadDouble(KD_ADDRESS);
}
double EEPROM_TC::getMac() {  // See issue #57 about this function
  return eepromReadDouble(MAC_ADDRESS);
}
double EEPROM_TC::getHeat() {
  return eepromReadDouble(HEAT_ADDRESS);
}
double EEPROM_TC::getAmplitude() {
  return eepromReadDouble(AMPLITUDE_ADDRESS);
}
double EEPROM_TC::getFrequency() {
  return eepromReadDouble(FREQUENCY_ADDRESS);
}
double EEPROM_TC::getGranularity() {
  return eepromReadDouble(GRANULARITY_ADDRESS);
}
double EEPROM_TC::getMaxDataAge() {
  return eepromReadDouble(MAX_DATA_AGE_ADDRESS);
}
double EEPROM_TC::getPHSeriesSize() {
  return eepromReadDouble(PH_SERIES_SIZE_ADDRESS);
}
double EEPROM_TC::getPHSeriesPointer() {
  return eepromReadDouble(PH_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getTempSeriesSize() {
  return eepromReadDouble(TEMP_SERIES_SIZE_ADDRESS);
}
double EEPROM_TC::getTempSeriesPointer() {
  return eepromReadDouble(TEMP_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getPHInterval() {
  return eepromReadDouble(PH_INTERVAL_ADDRESS);
}
double EEPROM_TC::getPHDelay() {
  return eepromReadDouble(PH_DELAY_ADDRESS);
}
double EEPROM_TC::getTempInterval() {
  return eepromReadDouble(TEMP_INTERVAL_ADDRESS);
}
double EEPROM_TC::getTempDelay() {
  return eepromReadDouble(TEMP_DELAY_ADDRESS);
}

// setter methods
void EEPROM_TC::setPH(double value) {
  eepromWriteDouble(PH_ADDRESS, value);
}
void EEPROM_TC::setTemp(double value) {
  eepromWriteDouble(TEMP_ADDRESS, value);
}
void EEPROM_TC::setTankID(double value) {
  eepromWriteDouble(TANK_ID_ADDRESS, value);
}
void EEPROM_TC::setCorrectedTemp(double value) {
  eepromWriteDouble(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC::setKP(double value) {
  eepromWriteDouble(KP_ADDRESS, value);
}
void EEPROM_TC::setKI(double value) {
  eepromWriteDouble(KI_ADDRESS, value);
}
void EEPROM_TC::setKD(double value) {
  eepromWriteDouble(KD_ADDRESS, value);
}
void EEPROM_TC::setMac(double value) {
  eepromWriteDouble(MAC_ADDRESS, value);
}
void EEPROM_TC::setHeat(double value) {
  eepromWriteDouble(HEAT_ADDRESS, value);
}
void EEPROM_TC::setAmplitude(double value) {
  eepromWriteDouble(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setFrequency(double value) {
  eepromWriteDouble(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC::setGranularity(double value) {
  eepromWriteDouble(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC::setMaxDataAge(double value) {
  eepromWriteDouble(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesSize(double value) {
  eepromWriteDouble(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesPointer(double value) {
  eepromWriteDouble(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesSize(double value) {
  eepromWriteDouble(TEMP_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesPointer(double value) {
  eepromWriteDouble(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setPHInterval(double value) {
  eepromWriteDouble(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setPHDelay(double value) {
  eepromWriteDouble(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC::setTempInterval(double value) {
  eepromWriteDouble(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setTempDelay(double value) {
  eepromWriteDouble(TEMP_DELAY_ADDRESS, value);
}
