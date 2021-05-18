#include "Devices/EEPROM_TC.h"

#include <EEPROM.h>

#include "Devices/Serial_TC.h"
#include "TC_util.h"
//  class variables
EEPROM_TC* EEPROM_TC::_instance = nullptr;

//  class methods
/**
 * accessor for singleton
 */
EEPROM_TC* EEPROM_TC::instance() {
  if (_instance) {
    return _instance;
  }
  _instance = new EEPROM_TC();
  Serial.println("EEPROM_TC");  // Serial_TC might not be ready yet
  return _instance;
}

//  instance methods
double EEPROM_TC::eepromReadDouble(int address) {
  assert(address >= 0);
  double value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

void EEPROM_TC::eepromWriteDouble(int address, double value) {
  assert(address >= 0);
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    EEPROM.update(address++, *p++);
  }
}

int EEPROM_TC::eepromReadInt(int address) {
  assert(address >= 0);
  int value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

void EEPROM_TC::eepromWriteInt(int address, int value) {
  assert(address >= 0);
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    EEPROM.update(address++, *p++);
  }
}

// getter methods
double EEPROM_TC::getAmplitude() {
  return eepromReadDouble(AMPLITUDE_ADDRESS);
}
double EEPROM_TC::getCorrectedTemp() {
  return eepromReadDouble(TEMP_CORR_ADDRESS);
}
double EEPROM_TC::getFrequency() {
  return eepromReadDouble(FREQUENCY_ADDRESS);
}
int EEPROM_TC::getGoogleSheetInterval() {
  return eepromReadInt(GOOGLE_INTERVAL_ADDRESS);
}
double EEPROM_TC::getGranularity() {
  return eepromReadDouble(GRANULARITY_ADDRESS);
}
bool EEPROM_TC::getHeat() {
  return (bool)eepromReadDouble(HEAT_ADDRESS);
}
double EEPROM_TC::getKD() {
  return eepromReadDouble(KD_ADDRESS);
}
double EEPROM_TC::getKI() {
  return eepromReadDouble(KI_ADDRESS);
}
double EEPROM_TC::getKP() {
  return eepromReadDouble(KP_ADDRESS);
}
double EEPROM_TC::getMac() {
  return eepromReadDouble(MAC_ADDRESS);
}
double EEPROM_TC::getMaxDataAge() {
  return eepromReadDouble(MAX_DATA_AGE_ADDRESS);
}
double EEPROM_TC::getPH() {
  return eepromReadDouble(PH_ADDRESS);
}
double EEPROM_TC::getPHDelay() {
  return eepromReadDouble(PH_DELAY_ADDRESS);
}
double EEPROM_TC::getPHInterval() {
  return eepromReadDouble(PH_INTERVAL_ADDRESS);
}
double EEPROM_TC::getPHSeriesPointer() {
  return eepromReadDouble(PH_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getPHSeriesSize() {
  return eepromReadDouble(PH_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC::getTankID() {
  return (int)eepromReadDouble(TANK_ID_ADDRESS);
}
double EEPROM_TC::getTemp() {
  return eepromReadDouble(TEMP_ADDRESS);
}
double EEPROM_TC::getTempDelay() {
  return eepromReadDouble(TEMP_DELAY_ADDRESS);
}
double EEPROM_TC::getTempInterval() {
  return eepromReadDouble(TEMP_INTERVAL_ADDRESS);
}
double EEPROM_TC::getTempSeriesPointer() {
  return eepromReadDouble(TEMP_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getTempSeriesSize() {
  return eepromReadDouble(TEMP_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC::getVersion() {
  return 2;  // we assume so!
}

// setter methods
void EEPROM_TC::setAmplitude(double value) {
  eepromWriteDouble(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setCorrectedTemp(double value) {
  eepromWriteDouble(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC::setFrequency(double value) {
  eepromWriteDouble(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC::setGoogleSheetInterval(int value) {
  eepromWriteInt(GOOGLE_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setGranularity(double value) {
  eepromWriteDouble(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC::setHeat(bool value) {
  eepromWriteDouble(HEAT_ADDRESS, (double)value);
}
void EEPROM_TC::setKD(double value) {
  eepromWriteDouble(KD_ADDRESS, value);
}
void EEPROM_TC::setKI(double value) {
  eepromWriteDouble(KI_ADDRESS, value);
}
void EEPROM_TC::setKP(double value) {
  eepromWriteDouble(KP_ADDRESS, value);
}
void EEPROM_TC::setMac(double value) {
  eepromWriteDouble(MAC_ADDRESS, value);
}
void EEPROM_TC::setMaxDataAge(double value) {
  eepromWriteDouble(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC::setPH(double value) {
  eepromWriteDouble(PH_ADDRESS, value);
}
void EEPROM_TC::setPHDelay(double value) {
  eepromWriteDouble(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC::setPHInterval(double value) {
  eepromWriteDouble(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesPointer(double value) {
  eepromWriteDouble(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesSize(double value) {
  eepromWriteDouble(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setTankID(int value) {
  eepromWriteDouble(TANK_ID_ADDRESS, (double)value);
}
void EEPROM_TC::setTemp(double value) {
  eepromWriteDouble(TEMP_ADDRESS, value);
}
void EEPROM_TC::setTempDelay(double value) {
  eepromWriteDouble(TEMP_DELAY_ADDRESS, value);
}
void EEPROM_TC::setTempInterval(double value) {
  eepromWriteDouble(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesPointer(double value) {
  eepromWriteDouble(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesSize(double value) {
  eepromWriteDouble(TEMP_SERIES_SIZE_ADDRESS, value);
}
