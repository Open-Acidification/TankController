#include "PHControl.h"

#include "Devices/DateTime_TC.h"
#include "Devices/EEPROM_TC.h"
#include "Devices/Serial_TC.h"
#include "PID_TC.h"
#include "TC_util.h"
#include "TankControllerLib.h"

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

PHControl::PHControl() {
  pinMode(PH_CONTROL_PIN, OUTPUT);
  digitalWrite(PH_CONTROL_PIN, TURN_SOLENOID_OFF);
  targetPh = EEPROM_TC::instance()->getPH();
  if (isnan(targetPh)) {
    targetPh = DEFAULT_PH;
    EEPROM_TC::instance()->setPH(targetPh);
  }
  serial("PHControl with target pH = %5.3f", targetPh);
}

void PHControl::setTargetPh(float newPh) {
  if (targetPh != newPh) {
    serial("change target pH from %6.4f to %6.4f", targetPh, newPh);
    targetPh = newPh;
    EEPROM_TC::instance()->setPH(newPh);
  }
}

void PHControl::enablePID(bool flag) {
  usePID = flag;
  // save to EEPROM?
  serial((flag ? "enable PID" : "disable PID"));
}

bool PHControl::isOn() {
  return digitalRead(PH_CONTROL_PIN) == TURN_SOLENOID_ON;
}

void PHControl::updateControl(float pH) {
  int msToBeOn;
  int nowModWindow = millis() % WINDOW_SIZE;
  COUT("PHControl::updateControl(" << pH << ") at " << millis());
  if (usePID) {
    msToBeOn = PID_TC::instance()->computeOutput(targetPh, pH);
    if (msToBeOn > 9000) {
      if (msToBeOn > 10000 && lastWarnMS + 60000 < millis()) {
        serial("WARNING: PID asked for an on time of %i which has been capped at 9000", msToBeOn);
        lastWarnMS = millis();
      }
      msToBeOn = 9000;
    }
  } else {
    msToBeOn = pH > targetPh ? 9000 : 0;
  }
  COUT("target: " << targetPh << "; current: " << pH << "; nowModWindow = " << nowModWindow
                  << "; msToBeOn = " << msToBeOn);
  bool newValue;
  if (TankControllerLib::instance()->isInCalibration()) {
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
    serial("CO2 bubbler turned %s after %lu ms", newValue ? "off" : "on", currentMS - lastSwitchMS);
    lastSwitchMS = currentMS;
  }
}
