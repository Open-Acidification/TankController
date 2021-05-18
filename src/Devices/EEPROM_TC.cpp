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
float EEPROM_TC::eepromReadFloat(int address) {
  assert(address >= 0);
  float value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

void EEPROM_TC::eepromWriteFloat(int address, float value) {
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
  return eepromReadFloat(AMPLITUDE_ADDRESS);
}
double EEPROM_TC::getCorrectedTemp() {
  return eepromReadFloat(TEMP_CORR_ADDRESS);
}
double EEPROM_TC::getFrequency() {
  return eepromReadFloat(FREQUENCY_ADDRESS);
}
int EEPROM_TC::getGoogleSheetInterval() {
  return eepromReadInt(GOOGLE_INTERVAL_ADDRESS);
}
double EEPROM_TC::getGranularity() {
  return eepromReadFloat(GRANULARITY_ADDRESS);
}
bool EEPROM_TC::getHeat() {
  return (bool)eepromReadFloat(HEAT_ADDRESS);
}
double EEPROM_TC::getKD() {
  return eepromReadFloat(KD_ADDRESS);
}
double EEPROM_TC::getKI() {
  return eepromReadFloat(KI_ADDRESS);
}
double EEPROM_TC::getKP() {
  return eepromReadFloat(KP_ADDRESS);
}
double EEPROM_TC::getMac() {
  return eepromReadFloat(MAC_ADDRESS);
}
double EEPROM_TC::getMaxDataAge() {
  return eepromReadFloat(MAX_DATA_AGE_ADDRESS);
}
double EEPROM_TC::getPH() {
  return eepromReadFloat(PH_ADDRESS);
}
double EEPROM_TC::getPHDelay() {
  return eepromReadFloat(PH_DELAY_ADDRESS);
}
double EEPROM_TC::getPHInterval() {
  return eepromReadFloat(PH_INTERVAL_ADDRESS);
}
double EEPROM_TC::getPHSeriesPointer() {
  return eepromReadFloat(PH_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getPHSeriesSize() {
  return eepromReadFloat(PH_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC::getTankID() {
  return (int)eepromReadFloat(TANK_ID_ADDRESS);
}
double EEPROM_TC::getTemp() {
  return eepromReadFloat(TEMP_ADDRESS);
}
double EEPROM_TC::getTempDelay() {
  return eepromReadFloat(TEMP_DELAY_ADDRESS);
}
double EEPROM_TC::getTempInterval() {
  return eepromReadFloat(TEMP_INTERVAL_ADDRESS);
}
double EEPROM_TC::getTempSeriesPointer() {
  return eepromReadFloat(TEMP_SERIES_POINTER_ADDRESS);
}
double EEPROM_TC::getTempSeriesSize() {
  return eepromReadFloat(TEMP_SERIES_SIZE_ADDRESS);
}
int EEPROM_TC::getVersion() {
  return 2;  // we assume so!
}

// setter methods
void EEPROM_TC::setAmplitude(double value) {
  eepromWriteFloat(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setCorrectedTemp(double value) {
  eepromWriteFloat(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC::setFrequency(double value) {
  eepromWriteFloat(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC::setGoogleSheetInterval(int value) {
  eepromWriteInt(GOOGLE_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setGranularity(double value) {
  eepromWriteFloat(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC::setHeat(bool value) {
  eepromWriteFloat(HEAT_ADDRESS, (double)value);
}
void EEPROM_TC::setKD(double value) {
  eepromWriteFloat(KD_ADDRESS, value);
}
void EEPROM_TC::setKI(double value) {
  eepromWriteFloat(KI_ADDRESS, value);
}
void EEPROM_TC::setKP(double value) {
  eepromWriteFloat(KP_ADDRESS, value);
}
void EEPROM_TC::setMac(double value) {
  eepromWriteFloat(MAC_ADDRESS, value);
}
void EEPROM_TC::setMaxDataAge(double value) {
  eepromWriteFloat(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC::setPH(double value) {
  eepromWriteFloat(PH_ADDRESS, value);
}
void EEPROM_TC::setPHDelay(double value) {
  eepromWriteFloat(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC::setPHInterval(double value) {
  eepromWriteFloat(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesPointer(double value) {
  eepromWriteFloat(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesSize(double value) {
  eepromWriteFloat(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setTankID(int value) {
  eepromWriteFloat(TANK_ID_ADDRESS, (double)value);
}
void EEPROM_TC::setTemp(double value) {
  eepromWriteFloat(TEMP_ADDRESS, value);
}
void EEPROM_TC::setTempDelay(double value) {
  eepromWriteFloat(TEMP_DELAY_ADDRESS, value);
}
void EEPROM_TC::setTempInterval(double value) {
  eepromWriteFloat(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesPointer(double value) {
  eepromWriteFloat(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesSize(double value) {
  eepromWriteFloat(TEMP_SERIES_SIZE_ADDRESS, value);
}
