#include "wrappers/EEPROM_TC.h"

#include <EEPROM.h>

#include "model/DataLogger.h"
#include "model/TC_util.h"
#include "wrappers/Serial_TC.h"

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
  float value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

/**
 * @brief writes the value to the address and triggers a remote log in DataLogger
 *
 * @param address
 * @param value
 */
void EEPROM_TC::eepromWriteFloat(uint16_t address, float value) {
  if (eepromReadFloat(address) != value) {
    byte* p = (byte*)(void*)&value;
    for (size_t i = 0; i < sizeof(value); i++) {
      EEPROM.write(address++, *p++);  // EEPROM.update() would perform read check before writing
    }
    DataLogger::instance()->writeWarningSoon();  // log all settings
  }
}

int32_t EEPROM_TC::eepromReadInt(uint16_t address) {
  int32_t value = 0;
  byte* p = (byte*)(void*)&value;
  for (size_t i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  return value;
}

/**
 * @brief writes the value to the address and triggers a remote log in DataLogger
 *
 * @param address
 * @param value
 */
void EEPROM_TC::eepromWriteInt(uint16_t address, int32_t value) {
  if (eepromReadInt(address) != value) {
    byte* p = (byte*)(void*)&value;
    for (size_t i = 0; i < sizeof(value); i++) {
      EEPROM.write(address++, *p++);  // EEPROM.update() would perform read check before writing
    }
    DataLogger::instance()->writeWarningSoon();  // log all settings
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
bool EEPROM_TC::getIgnoreBadThermalCorrection() {
  return (bool)eepromReadInt(IGNORE_BAD_THERMAL_CORRECTION_ADDRESS);
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
  return eepromReadInt(PH_RAMP_TIME_START_ADDRESS);
}
uint32_t EEPROM_TC::getPhRampTimeEnd() {
  return eepromReadInt(PH_RAMP_TIME_END_ADDRESS);
}
float EEPROM_TC::getRampStartingPh() {
  return eepromReadFloat(PH_RAMP_STARTING_ADDRESS);
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
uint32_t EEPROM_TC::getThermalRampTimeStart() {
  return eepromReadInt(THERMAL_RAMP_TIME_START_ADDRESS);
}
uint32_t EEPROM_TC::getThermalRampTimeEnd() {
  return eepromReadInt(THERMAL_RAMP_TIME_END_ADDRESS);
}
float EEPROM_TC::getThermalRampInitialValue() {
  return eepromReadFloat(THERMAL_RAMP_INITIAL_VALUE_ADDRESS);
}
uint32_t EEPROM_TC::getThermalSineStartTime() {
  return eepromReadInt(THERMAL_SINE_START_TIME_ADDRESS);
}
uint32_t EEPROM_TC::getThermalSinePeriod() {
  return eepromReadInt(THERMAL_SINE_AMPLITUDE_ADDRESS);
}
float EEPROM_TC::getThermalSineAmplitude() {
  return eepromReadFloat(THERMAL_SINE_PERIOD_ADDRESS);
}
uint16_t EEPROM_TC::getPHFunctionType() {
  return static_cast<uint16_t>(eepromReadFloat(PH_FUNCTION_TYPE_ADDRESS));
}
uint16_t EEPROM_TC::getThermalFunctionType() {
  return static_cast<uint16_t>(eepromReadFloat(THERMAL_FUNCTION_TYPE_ADDRESS));
}
uint16_t EEPROM_TC::getTankID() {
  return static_cast<uint16_t>(eepromReadFloat(TANK_ID_ADDRESS));
}
float EEPROM_TC::getThermalTarget() {
  return eepromReadFloat(THERMAL_TARGET_ADDRESS);
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
void EEPROM_TC::setIgnoreBadThermalCorrection(bool value) {
  eepromWriteInt(IGNORE_BAD_THERMAL_CORRECTION_ADDRESS, (int)value);
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
  eepromWriteInt(PH_RAMP_TIME_START_ADDRESS, value);
}
void EEPROM_TC::setPhRampTimeEnd(uint32_t value) {
  eepromWriteInt(PH_RAMP_TIME_END_ADDRESS, value);
}
void EEPROM_TC::setRampStartingPh(float value) {
  eepromWriteFloat(PH_RAMP_STARTING_ADDRESS, value);
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
void EEPROM_TC::setThermalRampTimeStart(uint32_t value) {
  eepromWriteInt(THERMAL_RAMP_TIME_START_ADDRESS, value);
}
void EEPROM_TC::setThermalRampTimeEnd(uint32_t value) {
  eepromWriteInt(THERMAL_RAMP_TIME_END_ADDRESS, value);
}
void EEPROM_TC::setThermalRampInitialValue(float value) {
  eepromWriteFloat(THERMAL_RAMP_INITIAL_VALUE_ADDRESS, value);
}
void EEPROM_TC::setThermalSineStartTime(uint32_t value) {
  eepromWriteInt(THERMAL_SINE_START_TIME_ADDRESS, value);
}
void EEPROM_TC::setThermalSinePeriod(uint32_t value) {
  eepromWriteInt(THERMAL_SINE_AMPLITUDE_ADDRESS, value);
}
void EEPROM_TC::setThermalSineAmplitude(float value) {
  eepromWriteFloat(THERMAL_SINE_PERIOD_ADDRESS, value);
}
void EEPROM_TC::setPHFunctionType(uint16_t value) {
  eepromWriteFloat(PH_FUNCTION_TYPE_ADDRESS, (float)value);
}
void EEPROM_TC::setThermalFunctionType(uint16_t value) {
  eepromWriteFloat(THERMAL_FUNCTION_TYPE_ADDRESS, (float)value);
}
void EEPROM_TC::setTankID(uint16_t value) {
  eepromWriteFloat(TANK_ID_ADDRESS, (float)value);
}
void EEPROM_TC::setTemp(float value) {
  eepromWriteFloat(THERMAL_TARGET_ADDRESS, value);
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

void EEPROM_TC::writeRemoteFileHeader(char* buffer, int size, int chunkNumber) {
  // rather than write an entire header line in one buffer, we break it into chunks to save memory
  switch (chunkNumber) {
    case 1:
      snprintf_P(buffer, size,
                 PSTR("Ignoring Bad pH Calibration\tTemperature Correction\tIgnoring Bad Temperature Calibration\tHeat "
                      "(1) or Chill (0)\tKD\tKI\tKP\tpH Flat (0) Ramp (1) Sine (2)\tpH Target\t"));
      break;
    case 2:
      snprintf_P(buffer, size,
                 PSTR("pH Ramp Start Time\tpH Ramp End Time\tpH Ramp Start Value\tpH Sine Start Time\tpH "
                      "Sine Period\tpH Sine Amplitude\tTemperature Flat (0) Ramp (1) Sine (2)\tTemperature Target\t"));
      break;
    case 3:
      snprintf_P(buffer, size,
                 PSTR("Temperature Ramp Start Time\tTemperature Ramp End Time\tTemperature Ramp "
                      "Start Value\tTemperature Sine Start Time\tTemperature Sine Period\tTemperature Sine "
                      "Amplitude\tGoogle Sheet Interval"));
      break;
    default:
      break;
  }
}

/**
 * @brief write settings to a string for logging purposes
 *
 * @param destination
 * @param size size of destination
 */
void EEPROM_TC::writeAllToString(char* destination, int size) {
  char thermalCorrectionString[7];
  float thermalCorrection = getThermalCorrection();
  if (isnan(thermalCorrection)) {
    snprintf_P(thermalCorrectionString, sizeof(thermalCorrectionString), PSTR("0.0"));
  } else {
    floattostrf(thermalCorrection, 1, 2, thermalCorrectionString, sizeof(thermalCorrectionString));
  }
  char kdString[9];
  float kd = getKD();
  if (isnan(kd)) {
    snprintf_P(kdString, sizeof(kdString), PSTR("0.0"));
  } else {
    floattostrf(kd, 1, 2, kdString, sizeof(kdString));
  }
  char kiString[9];
  float ki = getKI();
  if (isnan(ki)) {
    snprintf_P(kiString, sizeof(kiString), PSTR("0.0"));
  } else {
    floattostrf(ki, 1, 2, kiString, sizeof(kiString));
  }
  char kpString[10];
  float kp = getKP();
  if (isnan(kp)) {
    snprintf_P(kpString, sizeof(kpString), PSTR("0.0"));
  } else {
    floattostrf(kp, 1, 2, kpString, sizeof(kpString));
  }
  char pHTargetString[7];
  float pHTarget = getPh();
  if (isnan(pHTarget)) {
    snprintf_P(pHTargetString, sizeof(pHTargetString), PSTR("0.0"));
  } else {
    floattostrf(pHTarget, 1, 2, pHTargetString, sizeof(pHTargetString));
  }
  char rampStartingPhString[7];
  float rampStartingPh = getRampStartingPh();
  if (isnan(rampStartingPh)) {
    snprintf_P(rampStartingPhString, sizeof(rampStartingPhString), PSTR("0.0"));
  } else {
    floattostrf(rampStartingPh, 1, 2, rampStartingPhString, sizeof(rampStartingPhString));
  }
  char pHSineAmplitudeString[7];
  float pHSineAmplitude = getPhSineAmplitude();
  if (isnan(pHSineAmplitude)) {
    snprintf_P(pHSineAmplitudeString, sizeof(pHSineAmplitudeString), PSTR("0.0"));
  } else {
    floattostrf(pHSineAmplitude, 1, 2, pHSineAmplitudeString, sizeof(pHSineAmplitudeString));
  }
  char thermalTargetString[7];
  float thermalTarget = getThermalTarget();
  if (isnan(thermalTarget)) {
    snprintf_P(thermalTargetString, sizeof(thermalTargetString), PSTR("0.0"));
  } else {
    floattostrf(thermalTarget, 1, 2, thermalTargetString, sizeof(thermalTargetString));
  }
  char thermalRampInitialValueString[7];
  float thermalRampInitialValue = getThermalRampInitialValue();
  if (isnan(thermalRampInitialValue)) {
    snprintf_P(thermalRampInitialValueString, sizeof(thermalRampInitialValueString), PSTR("0.0"));
  } else {
    floattostrf(thermalRampInitialValue, 1, 2, thermalRampInitialValueString, sizeof(thermalRampInitialValueString));
  }
  char thermalSineAmplitudeString[7];
  float thermalSineAmplitude = getThermalSineAmplitude();
  if (isnan(thermalSineAmplitude)) {
    snprintf_P(thermalSineAmplitudeString, sizeof(thermalSineAmplitudeString), PSTR("0.0"));
  } else {
    floattostrf(thermalSineAmplitude, 1, 2, thermalSineAmplitudeString, sizeof(thermalSineAmplitudeString));
  }

  int length = snprintf_P(destination, size,
                          PSTR("%i\t%s\t%i\t%i\t%s\t%s\t%s\t%i\t%s\t%i\t%i\t%s\t%i\t%i\t%s\t%"
                               "i\t%s\t%i\t%i\t%s\t%i\t%i\t%s\t%i"),
                          (int)getIgnoreBadPHSlope(), thermalCorrectionString, (int)getIgnoreBadThermalCorrection(),
                          (int)getHeat(), kdString, kiString, kpString, (int)getPHFunctionType(), pHTargetString,
                          (int)getPhRampTimeStart(), (int)getPhRampTimeEnd(), rampStartingPhString,
                          (int)getPhSineStartTime(), (int)getPhSinePeriod(), pHSineAmplitudeString,
                          (int)getThermalFunctionType(), thermalTargetString, (int)getThermalRampTimeStart(),
                          (int)getThermalRampTimeEnd(), thermalRampInitialValueString, (int)getThermalSineStartTime(),
                          (int)getThermalSinePeriod(), thermalSineAmplitudeString, (int)getGoogleSheetInterval());
  if ((length > size) || (length < 0)) {
    // TODO: Log a warning that string was truncated
    serial(F("WARNING! String was truncated to \"%s\""), destination);
  }
}
