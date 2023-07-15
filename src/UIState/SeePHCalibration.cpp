/**
 * SeePHCalibration.cpp
 */

#include "SeePHCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

void SeePHCalibration::loop() {
  char buffer[20];
  PHProbe::instance()->getCalibration(buffer, sizeof(buffer));
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}

void SeePHCalibration::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine(F("requesting calib"), 1);
  PHProbe::instance()->sendCalibrationRequest();
}
