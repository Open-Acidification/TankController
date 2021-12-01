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
  Serial.println(F("EEPROM_TC"));  // Serial_TC might not be ready yet
  return _instance;
}

//  instance methods
float EEPROM_TC::eepromReadFloat(uint16_t address) {
  assert(address >= 0);
  float value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

void EEPROM_TC::eepromWriteFloat(uint16_t address, float value) {
  assert(address >= 0);
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    EEPROM.update(address++, *p++);
  }
}

int32_t EEPROM_TC::eepromReadInt(uint16_t address) {
  assert(address >= 0);
  int32_t value = 0;
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

void EEPROM_TC::eepromWriteInt(uint16_t address, int32_t value) {
  assert(address >= 0);
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    EEPROM.update(address++, *p++);
  }
}

// getter methods
float EEPROM_TC::getAmplitude() {
  return eepromReadFloat(AMPLITUDE_ADDRESS);
}
float EEPROM_TC::getCorrectedTemp() {
  return eepromReadFloat(TEMP_CORR_ADDRESS);
}
float EEPROM_TC::getFrequency() {
  return eepromReadFloat(FREQUENCY_ADDRESS);
}
uint16_t EEPROM_TC::getGoogleSheetInterval() {
  return eepromReadInt(GOOGLE_INTERVAL_ADDRESS);
}
float EEPROM_TC::getGranularity() {
  return eepromReadFloat(GRANULARITY_ADDRESS);
}
bool EEPROM_TC::getHeat() {
  return (bool)eepromReadFloat(HEAT_ADDRESS);
}
float EEPROM_TC::getKD() {
  return eepromReadFloat(KD_ADDRESS);
}
float EEPROM_TC::getKI() {
  return eepromReadFloat(KI_ADDRESS);
}
float EEPROM_TC::getKP() {
  return eepromReadFloat(KP_ADDRESS);
}
void EEPROM_TC::getMac(uint8_t* bytes) {
  for (int i = 0; i < 6; ++i) {
    bytes[i] = EEPROM.read(MAC_ADDRESS + i);  // Flawfinder: ignore
  }
}
float EEPROM_TC::getMaxDataAge() {
  return eepromReadFloat(MAX_DATA_AGE_ADDRESS);
}
float EEPROM_TC::getPH() {
  return eepromReadFloat(PH_ADDRESS);
}
float EEPROM_TC::getPHDelay() {
  return eepromReadFloat(PH_DELAY_ADDRESS);
}
float EEPROM_TC::getPHInterval() {
  return eepromReadFloat(PH_INTERVAL_ADDRESS);
}
float EEPROM_TC::getPHSeriesPointer() {
  return eepromReadFloat(PH_SERIES_POINTER_ADDRESS);
}
float EEPROM_TC::getPHSeriesSize() {
  return eepromReadFloat(PH_SERIES_SIZE_ADDRESS);
}
uint32_t EEPROM_TC::getRampTimeStart() {
  return eepromReadInt(RAMP_TIME_START_ADDRESS);
}
uint32_t EEPROM_TC::getRampTimeEnd() {
  return eepromReadInt(RAMP_TIME_END_ADDRESS);
}
float EEPROM_TC::getRampStartingPH() {
  return eepromReadFloat(RAMP_STARTING_PH_ADDRESS);
}
uint32_t EEPROM_TC::getRampTimeStartTemp() {
  return eepromReadInt(RAMP_TIME_START_TEMP_ADDRESS);
}
uint32_t EEPROM_TC::getRampTimeEndTemp() {
  return eepromReadInt(RAMP_TIME_END_TEMP_ADDRESS);
}
float EEPROM_TC::getRampStartingTemp() {
  return eepromReadFloat(RAMP_STARTING_TEMP_ADDRESS);
}
uint16_t EEPROM_TC::getTankID() {
  return static_cast<uint16_t>(eepromReadFloat(TANK_ID_ADDRESS));
}
float EEPROM_TC::getTemp() {
  return eepromReadFloat(TEMP_ADDRESS);
}
float EEPROM_TC::getTempDelay() {
  return eepromReadFloat(TEMP_DELAY_ADDRESS);
}
float EEPROM_TC::getTempInterval() {
  return eepromReadFloat(TEMP_INTERVAL_ADDRESS);
}
float EEPROM_TC::getTempSeriesPointer() {
  return eepromReadFloat(TEMP_SERIES_POINTER_ADDRESS);
}
float EEPROM_TC::getTempSeriesSize() {
  return eepromReadFloat(TEMP_SERIES_SIZE_ADDRESS);
}
// setter methods
void EEPROM_TC::setAmplitude(float value) {
  eepromWriteFloat(AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setCorrectedTemp(float value) {
  eepromWriteFloat(TEMP_CORR_ADDRESS, value);
}
void EEPROM_TC::setFrequency(float value) {
  eepromWriteFloat(FREQUENCY_ADDRESS, value);
}
void EEPROM_TC::setGoogleSheetInterval(uint16_t value) {
  eepromWriteInt(GOOGLE_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setGranularity(float value) {
  eepromWriteFloat(GRANULARITY_ADDRESS, value);
}
void EEPROM_TC::setHeat(bool value) {
  eepromWriteFloat(HEAT_ADDRESS, (float)value);
}
void EEPROM_TC::setKD(float value) {
  eepromWriteFloat(KD_ADDRESS, value);
}
void EEPROM_TC::setKI(float value) {
  eepromWriteFloat(KI_ADDRESS, value);
}
void EEPROM_TC::setKP(float value) {
  eepromWriteFloat(KP_ADDRESS, value);
}
void EEPROM_TC::setMac(uint8_t* bytes) {
  for (int i = 0; i < 6; ++i) {
    EEPROM.update(MAC_ADDRESS + i, bytes[i]);
  }
}
void EEPROM_TC::setMaxDataAge(float value) {
  eepromWriteFloat(MAX_DATA_AGE_ADDRESS, value);
}
void EEPROM_TC::setPH(float value) {
  eepromWriteFloat(PH_ADDRESS, value);
}
void EEPROM_TC::setPHDelay(float value) {
  eepromWriteFloat(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC::setPHInterval(float value) {
  eepromWriteFloat(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesPointer(float value) {
  eepromWriteFloat(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setPHSeriesSize(float value) {
  eepromWriteFloat(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setRampTimeStart(uint32_t value) {
  eepromWriteInt(RAMP_TIME_START_ADDRESS, value);
}
void EEPROM_TC::setRampTimeEnd(uint32_t value) {
  eepromWriteInt(RAMP_TIME_END_ADDRESS, value);
}
float EEPROM_TC::setRampStartingPH(float value) {
  uint16_t index = RAMP_STARTING_PH_ADDRESS;
  eepromWriteFloat(index, value);
}
void EEPROM_TC::setRampTimeStartTemp(uint32_t value) {
  eepromWriteInt(RAMP_TIME_START_TEMP_ADDRESS, value);
}
void EEPROM_TC::setRampTimeEndTemp(uint32_t value) {
  eepromWriteInt(RAMP_TIME_END_TEMP_ADDRESS, value);
}
float EEPROM_TC::setRampStartingTemp(float value) {
  eepromWriteFloat(RAMP_STARTING_TEMP_ADDRESS, value);
}
void EEPROM_TC::setTankID(uint16_t value) {
  eepromWriteFloat(TANK_ID_ADDRESS, (float)value);
}
void EEPROM_TC::setTemp(float value) {
  eepromWriteFloat(TEMP_ADDRESS, value);
}
void EEPROM_TC::setTempDelay(float value) {
  eepromWriteFloat(TEMP_DELAY_ADDRESS, value);
}
void EEPROM_TC::setTempInterval(float value) {
  eepromWriteFloat(TEMP_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesPointer(float value) {
  eepromWriteFloat(TEMP_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setTempSeriesSize(float value) {
  eepromWriteFloat(TEMP_SERIES_SIZE_ADDRESS, value);
}
