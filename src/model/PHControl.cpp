#include "model/PHControl.h"

#include <math.h>

#include "TankController.h"
#include "model/PHProbe.h"
#include "model/TC_util.h"
#include "wrappers/DateTime_TC.h"
#include "wrappers/EEPROM_TC.h"
#include "wrappers/PID_TC.h"
#include "wrappers/Serial_TC.h"

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
  baseTargetPh = EEPROM_TC::instance()->getPh();
  if (isnan(baseTargetPh)) {
    baseTargetPh = DEFAULT_PH;
    EEPROM_TC::instance()->setPh(baseTargetPh);
  }
  currentTargetPh = baseTargetPh;
  uint16_t pHFunctionInt = EEPROM_TC::instance()->getPHFunctionType();
  if (pHFunctionInt == 0xFFFF) {
    pHFunctionType = FLAT_TYPE;
    EEPROM_TC::instance()->setPHFunctionType((uint16_t)pHFunctionType);
  } else {
    pHFunctionType = (pHFunctionTypes)pHFunctionInt;
  }
  switch (pHFunctionType) {
    case RAMP_TYPE:
      rampTimeEndSeconds = EEPROM_TC::instance()->getPhRampTimeEnd();
      if (rampTimeEndSeconds == 0xFFFFFFFF || rampTimeEndSeconds == 0) {
        rampTimeEndSeconds = 0;
        EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEndSeconds);
        rampTimeStartSeconds = 0;
        EEPROM_TC::instance()->setPhRampTimeStart(rampTimeStartSeconds);
      } else {
        rampTimeStartSeconds = EEPROM_TC::instance()->getPhRampTimeStart();
        rampInitialValue = EEPROM_TC::instance()->getRampStartingPh();
      }
      break;
    case SINE_TYPE:
      periodInSeconds = EEPROM_TC::instance()->getPhSinePeriod();
      if (periodInSeconds == 0xFFFFFFFF || periodInSeconds == 0) {
        periodInSeconds = 12 * 60 * 60;  // 12 hours
        EEPROM_TC::instance()->setPhSinePeriod(periodInSeconds);
      }
      amplitude = EEPROM_TC::instance()->getPhSineAmplitude();
      sineStartTime = EEPROM_TC::instance()->getPhSineStartTime();
      serial(F("SINE_TYPE PHControl::PHControl() - sineStartTime = %lu"), sineStartTime / 3600);
      break;
    default:
      break;
  }
  char buffer[10];
  floattostrf(baseTargetPh, 5, 3, buffer, sizeof(buffer));
  serial(F("PHControl with base target pH = %s"), buffer);
}

void PHControl::setBaseTargetPh(float newPh) {
  if (baseTargetPh != newPh) {
    char buffer1[10];
    char buffer2[10];
    floattostrf(baseTargetPh, 5, 3, buffer1, sizeof(buffer1));
    floattostrf(newPh, 5, 3, buffer2, sizeof(buffer2));
    serial(F("change target pH from %s to %s"), buffer1, buffer2);
    baseTargetPh = newPh;
    EEPROM_TC::instance()->setPh(newPh);
  }
}

void PHControl::setRampDurationHours(float newPhRampDuration) {
  if (newPhRampDuration > 0) {
    float currentRampTime = rampTimeEndSeconds - rampTimeStartSeconds;
    char buffer1[10];
    char buffer2[10];
    floattostrf(currentRampTime, 5, 3, buffer1, sizeof(buffer1));
    floattostrf(newPhRampDuration, 5, 3, buffer2, sizeof(buffer2));
    serial(F("change ramp time from %s to %s"), buffer1, buffer2);
    rampTimeStartSeconds = DateTime_TC::now().secondstime();
    rampTimeEndSeconds = rampTimeStartSeconds + (uint32_t)(newPhRampDuration * 3600);
    rampInitialValue = PHProbe::instance()->getPh();
    pHFunctionType = pHFunctionTypes::RAMP_TYPE;
    EEPROM_TC::instance()->setPHFunctionType(pHFunctionType);
    EEPROM_TC::instance()->setPhRampTimeStart(rampTimeStartSeconds);
    EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEndSeconds);
    EEPROM_TC::instance()->setRampStartingPh(rampInitialValue);
  } else {
    rampTimeEndSeconds = 0;
    pHFunctionType = pHFunctionTypes::FLAT_TYPE;
    EEPROM_TC::instance()->setPHFunctionType(pHFunctionType);
    EEPROM_TC::instance()->setPhRampTimeEnd(rampTimeEndSeconds);
    serial("set ramp time to 0");
  }
}

void PHControl::setSineAmplitudeAndHours(float sineAmplitude, float sinePeriodInHours) {
  periodInSeconds = (sinePeriodInHours * 3600.0);
  amplitude = sineAmplitude;
  pHFunctionType = pHFunctionTypes::SINE_TYPE;
  sineStartTime = DateTime_TC::now().secondstime();
  EEPROM_TC::instance()->setPHFunctionType(pHFunctionType);
  EEPROM_TC::instance()->setPhSinePeriod(periodInSeconds);
  EEPROM_TC::instance()->setPhSineAmplitude(amplitude);
  EEPROM_TC::instance()->setPhSineStartTime(sineStartTime);
}

void PHControl::setSineAmplitude(float sineAmplitude) {
  assert(pHFunctionType == pHFunctionTypes::SINE_TYPE);
  amplitude = sineAmplitude;
  EEPROM_TC::instance()->setPhSineAmplitude(amplitude);
}

void PHControl::enablePID(bool flag) {
  usePID = flag;
  // save to EEPROM?
  serial(flag ? F("enable PID") : F("disable PID"));
}

bool PHControl::isOn() {
  return digitalRead(PH_CONTROL_PIN) == TURN_SOLENOID_ON;
}

void PHControl::loop() {
  float pH = PHProbe::instance()->getPh();
  int msToBeOn;
  int nowModWindow = millis() % WINDOW_SIZE;
  uint32_t currentTime = DateTime_TC::now().secondstime();
  switch (pHFunctionType) {
    case FLAT_TYPE: {
      currentTargetPh = baseTargetPh;
      break;
    }
    case RAMP_TYPE: {
      if (currentTime < rampTimeEndSeconds) {
        currentTargetPh = rampInitialValue + ((currentTime - rampTimeStartSeconds) * (baseTargetPh - rampInitialValue) /
                                              (rampTimeEndSeconds - rampTimeStartSeconds));
      } else {
        currentTargetPh = baseTargetPh;
      }
      break;
    }
    case SINE_TYPE: {
      uint32_t sineEndTime = sineStartTime + periodInSeconds;
      if (currentTime >= sineEndTime) {
        sineStartTime = DateTime_TC::now().secondstime();
        sineEndTime = sineStartTime + periodInSeconds;
        EEPROM_TC::instance()->setPhSineStartTime(sineStartTime);
      }
      float timeLeftTillPeriodEnd = sineEndTime - currentTime;
      float percentNOTThroughPeriod = timeLeftTillPeriodEnd / periodInSeconds;
      float percentThroughPeriod = 1 - percentNOTThroughPeriod;
      float x = percentThroughPeriod * (2 * PI);            // the x position for our sine wave
      currentTargetPh = amplitude * sin(x) + baseTargetPh;  // y position in our sine wave
      if (currentTargetPh > 14.0 || currentTargetPh < 0.0) {
        serial(F("WARNING: currentTargetPh = %i is out of range"), (int)currentTargetPh);
        currentTargetPh = 8.0;
      }
      break;
    }
    default:
      break;
  }
  if (usePID) {
    msToBeOn = PID_TC::instance()->computeOutput(currentTargetPh, pH);
    if (msToBeOn > 9000) {
      if (msToBeOn > 10000 && lastWarnMS + 60000 < millis()) {
        serial(F("WARNING: PID asked for an on time of %i which has been capped at 9000"), msToBeOn);
        lastWarnMS = millis();
      }
      msToBeOn = 9000;
    }
  } else {
    msToBeOn = pH > currentTargetPh ? 9000 : 0;
  }
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
