/**
 * PHCalibrationHigh.cpp
 */
#include "PHCalibrationHigh.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "SeePHCalibration.h"
#include "Wait.h"

void PHCalibrationHigh::setValue(float value) {
  PHProbe::instance()->setHighpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("New High="), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "New High=12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState(new Wait(tc, 3000, new SeePHCalibration(tc, true)));
}
