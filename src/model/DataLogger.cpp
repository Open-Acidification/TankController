#include "model/DataLogger.h"

#include "TankController.h"
#include "Version.h"
#include "model/PHControl.h"
#include "model/PHProbe.h"
#include "model/ThermalControl.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/Ethernet_TC.h"
#include "wrappers/PID_TC.h"
#include "wrappers/SD_TC.h"
#include "wrappers/Serial_TC.h"
#include "wrappers/ThermalProbe_TC.h"

// class variables
DataLogger* DataLogger::_instance = nullptr;

// class methods
/**
 * @brief accessor for singleton
 *
 */
DataLogger* DataLogger::instance() {
  if (!_instance) {
    _instance = new DataLogger();
  }
  return _instance;
}

// instance methods
/**
 * @brief check timers for logs that should be written
 *
 */
void DataLogger::loop() {
  unsigned long msNow = millis();
  if (msNow >= nextInfoLogTime) {
    writeInfoToLog();
    nextInfoLogTime = (msNow / (unsigned long)INFO_LOGGING_INTERVAL + 1) * (unsigned long)INFO_LOGGING_INTERVAL;
  } else if (msNow >= nextSDLogTime) {
    writeToSD();
    nextSDLogTime = (msNow / (unsigned long)SD_LOGGING_INTERVAL + 1) * (unsigned long)SD_LOGGING_INTERVAL;
  } else if (msNow >= nextSerialLogTime) {
    writeToSerial();
    nextSerialLogTime = (msNow / (unsigned long)SERIAL_LOGGING_INTERVAL + 1) * (unsigned long)SERIAL_LOGGING_INTERVAL;
  } else if (shouldWriteWarning) {
    writeWarningToLog();
    shouldWriteWarning = false;
  }
}

/**
 * @brief writes first four fields of status prefix to instance's buffer string
 *
 * @param severity 'D' for debug, 'I' for info, 'W' for warning, 'E' for error, 'F' for fatal
 */
void DataLogger::writePrefixToBuffer(const char severity) {
  // version \t tankid \t severity \t timestamp
  const __FlashStringHelper* format = F("%s\t%i\t%c\t%04i-%02i-%2i %02i:%02i:%02i");
  DateTime_TC dtNow = DateTime_TC::now();
  uint16_t tankId = EEPROM_TC::instance()->getTankID();
  int length = snprintf_P(buffer, sizeof(buffer), (PGM_P)format, VERSION, (uint16_t)tankId, severity,
                          (uint16_t)dtNow.year(), (uint16_t)dtNow.month(), (uint16_t)dtNow.day(),
                          (uint16_t)dtNow.hour(), (uint16_t)dtNow.minute(), (uint16_t)dtNow.second());
  if ((length > sizeof(buffer)) || (length < 0)) {
    // TODO: Log a warning that string was truncated
    serial(F("WARNING! String was truncated to \"%s\""), buffer);
  }
}

/**
 * @brief writes current values and targets for both temperature and pH to the status log
 *
 */
void DataLogger::writeInfoToLog() {
  char currentTemperatureString[10];
  char currentPhString[10];
  if (TankController::instance()->isInCalibration()) {
    strscpy_P(currentTemperatureString, F("C"), sizeof(currentTemperatureString));
    strscpy_P(currentPhString, F("C"), sizeof(currentPhString));
  } else {
    floattostrf((float)ThermalProbe_TC::instance()->getRunningAverage(), 1, 2, currentTemperatureString,
                sizeof(currentTemperatureString));
    floattostrf((float)PHProbe::instance()->getPh(), 1, 3, currentPhString, sizeof(currentPhString));
  }
  char thermalTarget[10];
  char pHTarget[10];
  floattostrf(ThermalControl::instance()->getCurrentThermalTarget(), 1, 2, thermalTarget, sizeof(thermalTarget));
  floattostrf(PHControl::instance()->getCurrentTargetPh(), 1, 3, pHTarget, sizeof(pHTarget));

  // write version, tankid, 'I', and timestamp to buffer
  writePrefixToBuffer('I');
  int prefixLength = strnlen(buffer, sizeof(buffer));
  // temperature \t thermaltarget \t pH \t pHtarget
  const __FlashStringHelper* format = F("\t%s\t%s\t%s\t%s");
  int additionalLength = snprintf_P(buffer + prefixLength, sizeof(buffer) - prefixLength, (PGM_P)format,
                                    currentTemperatureString, thermalTarget, currentPhString, pHTarget);
  if ((prefixLength + additionalLength > sizeof(buffer)) || (additionalLength < 0)) {
    // TODO: Log a warning that string was truncated
    serial(F("WARNING! String was truncated to \"%s\""), buffer);
  }
  SD_TC::instance()->appendToStatusLog(buffer);
  serial(F("New info written to log"));
}

/**
 * @brief write the current data to the log file on the SD
 *
 */
void DataLogger::writeToSD() {
  char currentTemperatureString[10];
  char currentPhString[10];
  if (TankController::instance()->isInCalibration()) {
    strscpy_P(currentTemperatureString, F("C"), sizeof(currentTemperatureString));
    strscpy_P(currentPhString, F("C"), sizeof(currentPhString));
  } else {
    floattostrf((float)ThermalProbe_TC::instance()->getRunningAverage(), 4, 2, currentTemperatureString,
                sizeof(currentTemperatureString));
    floattostrf((float)PHProbe::instance()->getPh(), 5, 3, currentPhString, sizeof(currentPhString));
  }
  DateTime_TC dtNow = DateTime_TC::now();
  PID_TC* pPID = PID_TC::instance();
  uint16_t tankId = EEPROM_TC::instance()->getTankID();
  char thermalTarget[10];
  char pHTarget[10];
  char kp[12];
  char ki[12];
  char kd[12];
  floattostrf(ThermalControl::instance()->getCurrentThermalTarget(), 4, 2, thermalTarget, sizeof(thermalTarget));
  floattostrf(PHControl::instance()->getCurrentTargetPh(), 5, 3, pHTarget, sizeof(pHTarget));
  floattostrf(pPID->getKp(), 8, 1, kp, sizeof(kp));
  floattostrf(pPID->getKi(), 8, 1, ki, sizeof(ki));
  floattostrf(pPID->getKd(), 8, 1, kd, sizeof(kd));
  const __FlashStringHelper* header = F("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime,Kp,Ki,Kd");
  const __FlashStringHelper* format = F("%02i/%02i/%4i %02i:%02i:%02i, %3i, %s, %s, %s, %s, %4lu, %s, %s, %s");
  char header_buffer[64];
  strscpy_P(header_buffer, header, sizeof(header_buffer));
  int length;
  length = snprintf_P(buffer, sizeof(buffer), (PGM_P)format, (uint16_t)dtNow.month(), (uint16_t)dtNow.day(),
                      (uint16_t)dtNow.year(), (uint16_t)dtNow.hour(), (uint16_t)dtNow.minute(),
                      (uint16_t)dtNow.second(), (uint16_t)tankId, currentTemperatureString, thermalTarget,
                      currentPhString, pHTarget, (unsigned long)(millis() / 1000), kp, ki, kd);
  if ((length > sizeof(buffer)) || (length < 0)) {
    // TODO: Log a warning that string was truncated
    serial(F("WARNING! String was truncated to \"%s\""), buffer);
  }
  SD_TC::instance()->appendData(header_buffer, buffer);
}

/**
 * @brief write the current data to the serial port
 *
 */
void DataLogger::writeToSerial() {
  DateTime_TC dtNow = DateTime_TC::now();
  char phString[12];
  char temperatureString[11];
  if (TankController::instance()->isInCalibration()) {
    strscpy_P(phString, F("C"), sizeof(phString));
    strscpy_P(temperatureString, F("C"), sizeof(temperatureString));
  } else {
    floattostrf((float)PHProbe::instance()->getPh(), 5, 3, phString, sizeof(phString));
    floattostrf((float)ThermalProbe_TC::instance()->getRunningAverage(), 5, 2, temperatureString,
                sizeof(temperatureString));
  }
  serial(F("%02d:%02d pH=%s temp=%s"), (uint16_t)dtNow.hour(), (uint16_t)dtNow.minute(), phString, temperatureString);
}

/**
 * @brief writes uptime, MAC address, pH slope, and EEPROM data to the status log
 *
 */
void DataLogger::writeWarningToLog() {
  uint32_t uptime = (unsigned long)(millis() / 1000);
  byte* mac = Ethernet_TC::instance()->getMac();

  // write version, tankid, 'W', and timestamp to buffer
  writePrefixToBuffer('W');
  int prefixLength = strnlen(buffer, sizeof(buffer));
  // uptime \t MACaddress \t pHslope \t
  const __FlashStringHelper* format = F("\t%lu\t%02X:%02X:%02X:%02X:%02X:%02X\t%s\t");
  int additionalLength = snprintf_P(buffer + prefixLength, sizeof(buffer) - prefixLength, (PGM_P)format, uptime, mac[0],
                                    mac[1], mac[2], mac[3], mac[4], mac[5], PHProbe::instance()->getSlopeResponse());
  if ((prefixLength + additionalLength > sizeof(buffer)) || (additionalLength < 0)) {
    // TODO: Log a warning that string was truncated
    serial(F("WARNING! String was truncated to \"%s\""), buffer);
  }
  // add EEPROM data
  EEPROM_TC::instance()->writeAllToString(buffer + prefixLength + additionalLength,
                                          sizeof(buffer) - prefixLength - additionalLength);
  SD_TC::instance()->appendToStatusLog(buffer);
  // serial(F("New warning written to log"));
}
