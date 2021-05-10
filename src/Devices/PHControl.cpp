#include "PHControl.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/Serial_TC.h"
#include "PID_TC.h"
#include "TC_util.h"
#include "TankControllerLib.h"

const double DEFAULT_PH = 8.1;

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
  window_start_time = millis();
  digitalWrite(PIN, HIGH);
  targetPh = EEPROM_TC::instance()->getPH();
  if (isnan(targetPh)) {
    targetPh = DEFAULT_PH;
    EEPROM_TC::instance()->setPH(targetPh);
  }
  serial("PHControl::PHControl() with target pH = %f", targetPh);
}

void PHControl::setTargetPh(double newPh) {
  if (targetPh != newPh) {
    serialWithTime("change target pH from %6.4f to %6.4f", targetPh, newPh);
    targetPh = newPh;
    EEPROM_TC::instance()->setPH(newPh);
  }
}

void PHControl::enablePID(bool flag) {
  usePID = flag;
  // save to EEPROM?
  serialWithTime((flag ? "enable PID" : "disable PID"));
}

void PHControl::updateControl(double pH) {
  if (usePID) {
    onTime = PID_TC::instance()->computeOutput(targetPh, pH);
  } else {
    onTime = pH > targetPh ? 10000 : 0;
  }
  long now = millis();
  if (now - window_start_time > WINDOW_SIZE) {  // time to shift the Relay Window
    window_start_time += WINDOW_SIZE;
    COUT("now: " << now << "; window_start_time: " << window_start_time);
  }
  COUT("target: " << targetPh << "; current: " << pH << "; onTime = " << onTime
                  << "; left = " << now - window_start_time);
  bool oldValue = digitalRead(PIN);
  bool newValue = oldValue;
  if (TankControllerLib::instance()->isInCalibration()) {
    newValue = HIGH;  // turn off CO2 while in calibration
  } else if ((onTime > SOLENOID_OPENING_TIME) && (onTime >= (now - window_start_time))) {
    newValue = LOW;  // open CO2 solenoid
  } else {
    newValue = HIGH;  // close CO2 solenoid
  }
  if (newValue != oldValue) {
    serialWithTime((newValue ? "CO2 bubbler off" : "CO2 bubbler on"));
    digitalWrite(PIN, newValue);
  }
}
