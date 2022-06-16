/**
 * SeeTemperature.cpp
 */

#include "SeeTemperature.h"

#include "Devices/DateTime_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TempProbe_TC.h"
#include "Devices/TemperatureControl.h"
#include "TC_util.h"

void SeeTemperature::loop() {
  int32_t oddEven = ((millis() - startTime) / 3000) % 2;
  switch (oddEven) {
    case 0:
      loadHeader(0);
      loadValues(1);
      break;
    case 1:
      loadTempSetType(0);
      loadTypeVariables(1);
      break;
  }
}

void SeeTemperature::loadTempSetType(uint16_t line) {
  char buffer[17];
  char *type;
  switch (TemperatureControl::instance()->getTempSetType()) {
    case FLAT_TYPE: {
      type = (char *)"flat";
      break;
    }
    case RAMP_TYPE: {
      type = (char *)"ramp";
      break;
    }
    case SINE_TYPE: {
      type = (char *)"sine";
      break;
    }
    default: {
      type = (char *)"????";
      break;
    }
  }
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("type: %s"), type);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeeTemperature::loadTypeVariables(uint16_t line) {
  char buffer[17];
  switch (TemperatureControl::instance()->getTempSetType()) {
    case FLAT_TYPE: {
      break;
    }
    case RAMP_TYPE: {
      uint32_t endTime = TemperatureControl::instance()->getRampTimeEnd();
      uint32_t currentTime = DateTime_TC::now().secondstime();
      int timeLeft = endTime - currentTime;
      timeLeft = timeLeft > 0 ? timeLeft : 0;
      int timeLeftMinutes = timeLeft / 60;
      int timeLeftHours = timeLeft / 3600;
      int timeLeftSeconds = timeLeft % 60;
      snprintf_P(buffer, sizeof(buffer), (PGM_P)F("left: %i:%i:%i"), timeLeftHours, timeLeftMinutes, timeLeftSeconds);
      LiquidCrystal_TC::instance()->writeLine(buffer, line);
      break;
    }
    case SINE_TYPE: {
      uint32_t period = TemperatureControl::instance()->getPeriod();
      float periodHours = period / 3600.0;
      float amplitude = TemperatureControl::instance()->getAmplitude();

      snprintf_P(buffer, sizeof(buffer), (PGM_P)F("p=%i.%03i a=%i.%03i"), (int)periodHours,
                 (int)(periodHours * 1000) % 1000, (int)amplitude, (int)(amplitude * 1000) % 1000);
      LiquidCrystal_TC::instance()->writeLine(buffer, line);
      break;
    }
    default:
      break;
  }
}

void SeeTemperature::loadHeader(uint16_t line) {
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("Now  Next  Goal"));
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}

void SeeTemperature::loadValues(uint16_t line) {
  char buffer[17];
  float overallTargetTemp = TemperatureControl::instance()->getTargetTemperature();
  float currentTargetTemp = TemperatureControl::instance()->getCurrentTemperatureTarget();
  float currentTemp = TempProbe_TC::instance()->getRunningAverage();
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i.%01i %i.%02i %i.%02i"), (int)currentTemp,
             (int)(currentTemp * 10 + 0.5) % 10, (int)currentTargetTemp, (int)(currentTargetTemp * 100 + 0.5) % 100,
             (int)overallTargetTemp, (int)(overallTargetTemp * 100) % 100);
  LiquidCrystal_TC::instance()->writeLine(buffer, line);
}
