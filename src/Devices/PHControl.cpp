#include "PHControl.h"

#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "PID_TC.h"
#include "TC_util.h"
#include "TankController.h"

const float DEFAULT_PH = 8.1;

//  class instance variables
/**
 * static variable for singleton
 */
PHControl *PHControl::_instance = nullptr;

//  class methods
/**
 * static member function to return singleton
 */
PHControl *PHControl::instance() {
  if (!_instance) {
    _instance = new PHControl();
  }
  return _instance;
}

void PHControl::clearInstance() {
  if (_instance) {
    delete _instance;
    _instance = nullptr;
  }
}

PHControl::PHControl() {
  pinMode(PH_CONTROL_PIN, OUTPUT);
  digitalWrite(PH_CONTROL_PIN, TURN_SOLENOID_OFF);
  targetPh = EEPROM_TC::instance()->getPH();
  if (isnan(targetPh)) {
    targetPh = DEFAULT_PH;
    EEPROM_TC::instance()->setPH(targetPh);
  }
  rampTimeEnd = EEPROM_TC::instance()->getRampTimeEnd();
  if (rampTimeEnd == 0xFFFFFFFF || rampTimeEnd == 0) {
    rampTimeEnd = 0;
    EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEnd);
    rampTimeStart = 0;
    EEPROM_TC::instance()->setPhRampTimeStart(rampTimeStart);
  } else {
    rampTimeStart = EEPROM_TC::instance()->getRampTimeStart();
    rampStartingPh = EEPROM_TC::instance()->getRampStartingPH();
  }
  char buffer[40];
  strncpy_P(buffer, (PGM_P)F("PHControl with target pH = "), sizeof(buffer));
  dtostrf(targetPh, 5, 3, buffer + strnlen(buffer, sizeof(buffer)));
  serial(buffer);
}

void PHControl::setTargetPh(float newPh) {
  if (targetPh != newPh) {
    char buffer[40];
    strncpy_P(buffer, (PGM_P)F("change target pH from "), sizeof(buffer));
    dtostrf(targetPh, 5, 3, buffer + strnlen(buffer, sizeof(buffer)));
    strcpy_P(buffer + strnlen(buffer, sizeof(buffer)), (PGM_P)F(" to "));
    dtostrf(newPh, 5, 3, buffer + strnlen(buffer, sizeof(buffer)));
    serial(buffer);
    targetPh = newPh;
    EEPROM_TC::instance()->setPH(newPh);
  }
}

void PHControl::setRampDuration(float newPhRampDuration) {
  if (newPhRampDuration > 0) {
    char buffer[40];
    float currentRampTime = rampTimeEnd - rampTimeStart;
    strncpy_P(buffer, (PGM_P)F("change ramp time from "), sizeof(buffer));
    dtostrf(currentRampTime, 5, 3, buffer + strnlen(buffer, sizeof(buffer)));
    strcpy_P(buffer + strnlen(buffer, sizeof(buffer)), (PGM_P)F(" to "));
    dtostrf(newPhRampDuration, 5, 3, buffer + strnlen(buffer, sizeof(buffer)));
    serial(buffer);
    rampTimeStart = DateTime_TC::now().secondstime();
    rampTimeEnd = rampTimeStart + (newPhRampDuration * 3600);
    rampStartingPh = PHProbe::instance()->getPh();
    EEPROM_TC::instance()->setPhRampTimeStart(rampTimeStart);
    EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEnd);
    EEPROM_TC::instance()->setRampStartingPH(rampStartingPh);
  } else {
    rampTimeEnd = 0;
    EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEnd);
    serial("set ramp time to 0");
  }
}

void PHControl::enablePID(bool flag) {
  usePID = flag;
  // save to EEPROM?
  serial(flag ? F("enable PID") : F("disable PID"));
}

bool PHControl::isOn() {
  return digitalRead(PH_CONTROL_PIN) == TURN_SOLENOID_ON;
}

void PHControl::updateControl(float pH) {
  int msToBeOn;
  int nowModWindow = millis() % WINDOW_SIZE;
  float currentTime = DateTime_TC::now().secondstime();
  // if ramp is being used
  if (currentTime < rampTimeEnd) {
    currentPHTarget =
        rampStartingPh + ((currentTime - rampTimeStart) * (targetPh - rampStartingPh) / (rampTimeEnd - rampTimeStart));
  } else {
    currentPHTarget = targetPh;
  }
  COUT("PHControl::updateControl(" << pH << ") at " << millis());
  if (usePID) {
    msToBeOn = PID_TC::instance()->computeOutput(currentPHTarget, pH);
    if (msToBeOn > 9000) {
      if (msToBeOn > 10000 && lastWarnMS + 60000 < millis()) {
        serial(F("WARNING: PID asked for an on time of %i which has been capped at 9000"), msToBeOn);
        lastWarnMS = millis();
      }
      msToBeOn = 9000;
    }
  } else {
    msToBeOn = pH > currentPHTarget ? 9000 : 0;
  }
  COUT("target: " << currentPHTarget << "; current: " << pH << "; nowModWindow = " << nowModWindow
                  << "; msToBeOn = " << msToBeOn);
  bool newValue;
  if (TankController::instance()->isInCalibration()) {
    COUT("pH control should be off since in calibration");
    newValue = TURN_SOLENOID_OFF;  // turn off CO2 while in calibration
  } else if (SOLENOID_OPENING_TIME < msToBeOn && nowModWindow < msToBeOn) {
    COUT("pH control should be on");
    newValue = TURN_SOLENOID_ON;  // open CO2 solenoid
  } else {
    COUT("pH control should be off");
    newValue = TURN_SOLENOID_OFF;  // close CO2 solenoid
  }
  if (newValue != digitalRead(PH_CONTROL_PIN)) {
    digitalWrite(PH_CONTROL_PIN, newValue);
    uint32_t currentMS = millis();
    serial(F("CO2 bubbler turned %s after %lu ms"), newValue ? "off" : "on", currentMS - lastSwitchMS);
    lastSwitchMS = currentMS;
  }
}
