#include "model/DataLogger.h"

#include "TankController.h"
#include "Version.h"
#include "model/PHControl.h"
#include "model/PHProbe.h"
#include "model/ThermalControl.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
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
  }
}

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
  DateTime_TC dtNow = DateTime_TC::now();
  PID_TC* pPID = PID_TC::instance();
  uint16_t tankId = EEPROM_TC::instance()->getTankID();
  char thermalTarget[10];
  char pHTarget[10];
  char kp[12];
  char ki[12];
  char kd[12];
  floattostrf(ThermalControl::instance()->getCurrentThermalTarget(), 1, 2, thermalTarget, sizeof(thermalTarget));
  floattostrf(PHControl::instance()->getCurrentTargetPh(), 1, 3, pHTarget, sizeof(pHTarget));
  floattostrf(pPID->getKp(), 1, 1, kp, sizeof(kp));
  floattostrf(pPID->getKi(), 1, 1, ki, sizeof(ki));
  floattostrf(pPID->getKd(), 1, 1, kd, sizeof(kd));
  // version,tankid,time,thermal value,thermal target,pH,pH target,uptime in seconds,Kp,Ki,Kd
  const __FlashStringHelper* format = F("%s\t%i\t%02i/%02i/%4i %02i:%02i:%02i\t%s\t%s\t%s\t%s\t%lu\t%s\t%s\t%s");
  int length;
  length = snprintf_P(buffer, sizeof(buffer), (PGM_P)format, VERSION, (uint16_t)tankId, (uint16_t)dtNow.month(),
                      (uint16_t)dtNow.day(), (uint16_t)dtNow.year(), (uint16_t)dtNow.hour(), (uint16_t)dtNow.minute(),
                      (uint16_t)dtNow.second(), currentTemperatureString, thermalTarget, currentPhString, pHTarget,
                      (unsigned long)(millis() / 1000), kp, ki, kd);
  if ((length > sizeof(buffer)) || (length < 0)) {
    // TODO: Log a warning that string was truncated
    serial(F("WARNING! String was truncated to \"%s\""), buffer);
  }
  SD_TC::instance()->appendInfo(buffer);
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
