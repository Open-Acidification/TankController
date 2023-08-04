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
float EEPROM_TC::getThermalCorrection() {
  return eepromReadFloat(THERMAL_CORRECTION_ADDRESS);
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
bool EEPROM_TC::getIgnoreBadPHSlope() {
  return (bool)eepromReadInt(IGNORE_BAD_PH_SLOPE_ADDRESS);
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
float EEPROM_TC::getPh() {
  return eepromReadFloat(PH_ADDRESS);
}
float EEPROM_TC::getPhDelay() {
  return eepromReadFloat(PH_DELAY_ADDRESS);
}
float EEPROM_TC::getPhInterval() {
  return eepromReadFloat(PH_INTERVAL_ADDRESS);
}
float EEPROM_TC::getPhSeriesPointer() {
  return eepromReadFloat(PH_SERIES_POINTER_ADDRESS);
}
float EEPROM_TC::getPhSeriesSize() {
  return eepromReadFloat(PH_SERIES_SIZE_ADDRESS);
}
uint32_t EEPROM_TC::getPhRampTimeStart() {
  return eepromReadInt(RAMP_TIME_START_PH_ADDRESS);
}
uint32_t EEPROM_TC::getPhRampTimeEnd() {
  return eepromReadInt(RAMP_TIME_END_PH_ADDRESS);
}
float EEPROM_TC::getRampStartingPh() {
  return eepromReadFloat(RAMP_STARTING_PH_ADDRESS);
}
uint32_t EEPROM_TC::getPhSineStartTime() {
  return eepromReadInt(PH_SINE_START_TIME_ADDRESS);
}
uint32_t EEPROM_TC::getPhSinePeriod() {
  return eepromReadInt(PH_SINE_AMPLITUDE_ADDRESS);
}
float EEPROM_TC::getPhSineAmplitude() {
  return eepromReadFloat(PH_SINE_PERIOD_ADDRESS);
}
uint32_t EEPROM_TC::getTempRampTimeStart() {
  return eepromReadInt(RAMP_TIME_START_ADDRESS);
}
uint32_t EEPROM_TC::getTempRampTimeEnd() {
  return eepromReadInt(RAMP_TIME_END_ADDRESS);
}
float EEPROM_TC::getRampStartingTemp() {
  return eepromReadFloat(RAMP_STARTING_TEMP_ADDRESS);
}
uint32_t EEPROM_TC::getTempSineStartTime() {
  return eepromReadInt(TEMP_SINE_START_TIME_ADDRESS);
}
uint32_t EEPROM_TC::getTempSinePeriod() {
  return eepromReadInt(TEMP_SINE_AMPLITUDE_ADDRESS);
}
float EEPROM_TC::getTempSineAmplitude() {
  return eepromReadFloat(TEMP_SINE_PERIOD_ADDRESS);
}
uint16_t EEPROM_TC::getPhSetType() {
  return static_cast<uint16_t>(eepromReadFloat(PH_SET_TYPE_ADDRESS));
}
uint16_t EEPROM_TC::getThermalFunctionType() {
  return static_cast<uint16_t>(eepromReadFloat(THERMAL_FUNCTION_TYPE_ADDRESS));
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
void EEPROM_TC::setThermalCorrection(float value) {
  eepromWriteFloat(THERMAL_CORRECTION_ADDRESS, value);
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
void EEPROM_TC::setIgnoreBadPHSlope(bool value) {
  eepromWriteInt(IGNORE_BAD_PH_SLOPE_ADDRESS, (int)value);
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
void EEPROM_TC::setPh(float value) {
  eepromWriteFloat(PH_ADDRESS, value);
}
void EEPROM_TC::setPhDelay(float value) {
  eepromWriteFloat(PH_DELAY_ADDRESS, value);
}
void EEPROM_TC::setPhInterval(float value) {
  eepromWriteFloat(PH_INTERVAL_ADDRESS, value);
}
void EEPROM_TC::setPhSeriesPointer(float value) {
  eepromWriteFloat(PH_SERIES_POINTER_ADDRESS, value);
}
void EEPROM_TC::setPhSeriesSize(float value) {
  eepromWriteFloat(PH_SERIES_SIZE_ADDRESS, value);
}
void EEPROM_TC::setPhRampTimeStart(uint32_t value) {
  eepromWriteInt(RAMP_TIME_START_PH_ADDRESS, value);
}
void EEPROM_TC::setPhRampTimeEnd(uint32_t value) {
  eepromWriteInt(RAMP_TIME_END_PH_ADDRESS, value);
}
void EEPROM_TC::setRampStartingPh(float value) {
  eepromWriteFloat(RAMP_STARTING_PH_ADDRESS, value);
}
void EEPROM_TC::setPhSineStartTime(uint32_t value) {
  eepromWriteInt(PH_SINE_START_TIME_ADDRESS, value);
}
void EEPROM_TC::setPhSinePeriod(uint32_t value) {
  eepromWriteInt(PH_SINE_AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setPhSineAmplitude(float value) {
  eepromWriteFloat(PH_SINE_PERIOD_ADDRESS, value);
}
void EEPROM_TC::setTempRampTimeStart(uint32_t value) {
  eepromWriteInt(RAMP_TIME_START_ADDRESS, value);
}
void EEPROM_TC::setTempRampTimeEnd(uint32_t value) {
  eepromWriteInt(RAMP_TIME_END_ADDRESS, value);
}
void EEPROM_TC::setRampStartingTemp(float value) {
  eepromWriteFloat(RAMP_STARTING_TEMP_ADDRESS, value);
}
void EEPROM_TC::setTempSineStartTime(uint32_t value) {
  eepromWriteInt(TEMP_SINE_START_TIME_ADDRESS, value);
}
void EEPROM_TC::setTempSinePeriod(uint32_t value) {
  eepromWriteInt(TEMP_SINE_AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setTempSineAmplitude(float value) {
  eepromWriteFloat(TEMP_SINE_PERIOD_ADDRESS, value);
}
void EEPROM_TC::setPhSetType(uint16_t value) {
  eepromWriteFloat(PH_SET_TYPE_ADDRESS, (float)value);
}
void EEPROM_TC::setThermalFunctionType(uint16_t value) {
  eepromWriteFloat(THERMAL_FUNCTION_TYPE_ADDRESS, (float)value);
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
