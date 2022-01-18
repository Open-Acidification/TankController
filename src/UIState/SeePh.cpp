/**
 * SeePIDConstants.cpp
 */

#include "SeePh.h"

#include "Devices/DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "Devices/PHProbe.h"
#include "TC_util.h"

void SeePh::loop() {
  int32_t elapsedSeconds = ((millis() - startTime) / 3000) % 2;
  switch (elapsedSeconds) {
  case 0:
    loadTarget(0);
    loadCurrent(1);
    break;
  case 1:
    loadPhSetType(0);
    loadTypeVariables(1);
    break;
  }
}

void SeePh::loadPhSetType(uint16_t line) {
  char buffer[17];
  // int value = PHControl::instance()->getPhSetType();
  char *type;
  switch (PHControl::instance()->getPhSetType()) {
  case FLAT_TYPE: {
    type = (char *)"flat";
    break;
  }
  case RAMP_TYPE: {
    type = (char *)"ramp";
    break;
  }
  case SINE_TYPE: {
    type = (char *)"Sine";
    break;
  }
  default:
    break;
  }
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("type: %s"), type);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePh::loadTypeVariables(uint16_t line) {
  char buffer[17];
  switch (PHControl::instance()->getPhSetType()) {
  case FLAT_TYPE: {
    break;
  }
  case RAMP_TYPE: {
    uint32_t endTime = PHControl::instance()->getPhRampTimeEnd();
    uint32_t currentTime = DateTime_TC::now().secondstime();
    float timeLeft = endTime - currentTime;
    float timeLeftHours = timeLeft / 3600;
    if (timeLeft > 0) {
      snprintf_P(buffer, sizeof(buffer), (PGM_P)F("hrs left: %i.%i"),
                 (int)timeLeftHours, (int)(timeLeftHours * 1000) % 1000);
      LiquidCrystal_TC::instance()->writeLine(buffer, line);
    } else {
      snprintf_P(buffer, sizeof(buffer), (PGM_P)F("hrs left: %i.%i"), 0, 0);
      LiquidCrystal_TC::instance()->writeLine(buffer, line);
    }
    break;
  }
  case SINE_TYPE: {
    uint32_t period = PHControl::instance()->getPeriod();
    float periodHours = period / 3600;
    float amplitude = PHControl::instance()->getAmplitude();

    snprintf_P(buffer, sizeof(buffer), (PGM_P)F("p=%i.%i a=%i.%i"),
               (int)periodHours, (int)(periodHours * 1000) % 1000,
               (int)amplitude, (int)(amplitude * 1000) % 1000);
    LiquidCrystal_TC::instance()->writeLine(buffer, line);
    break;
  }
  default:
    break;
  }
}

void SeePh::loadTarget(uint16_t line) {
  char buffer[17];
  float TargetPh = PHControl::instance()->getTargetPh();
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Target pH=%i.%i"), (int)TargetPh,
             (int)(TargetPh * 1000) % 1000);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeePh::loadCurrent(uint16_t line) {
  char buffer[17];
  float currentPh = PHProbe::instance()->getPh();
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Actual pH=%i.%i"),
             (int)currentPh, (int)(currentPh * 1000) % 1000);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}
