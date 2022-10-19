#include "PHControl.h"

#include <math.h>

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
  targetPh = EEPROM_TC::instance()->getPh();
  if (isnan(targetPh)) {
    targetPh = DEFAULT_PH;
    EEPROM_TC::instance()->setPh(targetPh);
  }
  pHSetType = EEPROM_TC::instance()->getPhSetType();
  if (pHSetType == 0xFFFFFFFF) {
    pHSetType = FLAT_TYPE;
    EEPROM_TC::instance()->setPhSetType(pHSetType);
  }
  switch (pHSetType) {
    case RAMP_TYPE:
      rampTimeEnd = EEPROM_TC::instance()->getPhRampTimeEnd();
      if (rampTimeEnd == 0xFFFFFFFF || rampTimeEnd == 0) {
        rampTimeEnd = 0;
        EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEnd);
        rampTimeStart = 0;
        EEPROM_TC::instance()->setPhRampTimeStart(rampTimeStart);
      } else {
        rampTimeStart = EEPROM_TC::instance()->getPhRampTimeStart();
        rampStartingPh = EEPROM_TC::instance()->getRampStartingPh();
      }
      break;
    case SINE_TYPE:
      period = EEPROM_TC::instance()->getPhSinePeriod();
      amplitude = EEPROM_TC::instance()->getPhSineAmplitude();
      sineStartTime = EEPROM_TC::instance()->getPhSineStartTime();
      break;
    default:
      break;
  }
  char buffer[10];
  floattostrf(targetPh, 5, 3, buffer, sizeof(buffer));
  serial(F("PHControl with target pH = %s"), buffer);
}

void PHControl::setTargetPh(float newPh) {
  if (targetPh != newPh) {
    char buffer1[10];
    char buffer2[10];
    floattostrf(targetPh, 5, 3, buffer1, sizeof(buffer1));
    floattostrf(newPh, 5, 3, buffer2, sizeof(buffer2));
    serial(F("change target pH from %s to %s"), buffer1, buffer2);
    targetPh = newPh;
    EEPROM_TC::instance()->setPh(newPh);
  }
}

void PHControl::setRampDuration(float newPhRampDuration) {
  if (newPhRampDuration > 0) {
    float currentRampTime = rampTimeEnd - rampTimeStart;
    char buffer1[10];
    char buffer2[10];
    floattostrf(currentRampTime, 5, 3, buffer1, sizeof(buffer1));
    floattostrf(newPhRampDuration, 5, 3, buffer2, sizeof(buffer2));
    serial(F("change ramp time from %s to %s"), buffer1, buffer2);
    rampTimeStart = DateTime_TC::now().secondstime();
    rampTimeEnd = rampTimeStart + (uint32_t)(newPhRampDuration * 3600);
    rampStartingPh = PHProbe::instance()->getPh();
    pHSetType = phSetTypeTypes::RAMP_TYPE;
    EEPROM_TC::instance()->setPhSetType(pHSetType);
    EEPROM_TC::instance()->setPhRampTimeStart(rampTimeStart);
    EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEnd);
    EEPROM_TC::instance()->setRampStartingPh(rampStartingPh);
  } else {
    rampTimeEnd = 0;
    pHSetType = phSetTypeTypes::FLAT_TYPE;
    EEPROM_TC::instance()->setPhSetType(pHSetType);
    EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEnd);
    serial("set ramp time to 0");
  }
}

void PHControl::setSine(float sineAmplitude, float sinePeriodInHours) {
  period = (sinePeriodInHours * 3600);
  amplitude = sineAmplitude;
  pHSetType = phSetTypeTypes::SINE_TYPE;
  sineStartTime = DateTime_TC::now().secondstime();
  EEPROM_TC::instance()->setPhSetType(pHSetType);
  EEPROM_TC::instance()->setPhSinePeriod(period);
  EEPROM_TC::instance()->setPhSineAmplitude(amplitude);
  EEPROM_TC::instance()->setPhSineStartTime(sineStartTime);
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
  uint32_t currentTime = DateTime_TC::now().secondstime();
  switch (pHSetType) {
    case FLAT_TYPE: {
      currentPHTarget = targetPh;
      break;
    }
    case RAMP_TYPE: {
      if (currentTime < rampTimeEnd) {
        currentPHTarget = rampStartingPh +
                          ((currentTime - rampTimeStart) * (targetPh - rampStartingPh) / (rampTimeEnd - rampTimeStart));
      } else {
        currentPHTarget = targetPh;
      }
      break;
    }
    case SINE_TYPE: {
      uint32_t sineEndTime = sineStartTime + period;
      if (currentTime >= sineEndTime) {
        sineStartTime = DateTime_TC::now().secondstime();
        sineEndTime = sineStartTime + period;
        EEPROM_TC::instance()->setPhSineStartTime(sineStartTime);
      }
      float timeLeftTillPeriodEnd = sineEndTime - currentTime;
      float percentNOTThroughPeriod = timeLeftTillPeriodEnd / period;
      float percentThroughPeriod = 1 - percentNOTThroughPeriod;
      float x = percentThroughPeriod * (2 * PI);        // the x position for our sine wave
      currentPHTarget = amplitude * sin(x) + targetPh;  // y position in our sine wave
      break;
    }
    default:
      break;
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
