/**
 * PHCalibrationLow.cpp
 */
#include "PHCalibrationLow.h"

#include "SeePHCalibration.h"
#include "UIState.h"
#include "Wait.h"
#include "model/PHProbe.h"
#include "wrappers/LiquidCrystal_TC.h"

void PHCalibrationLow::setValue(float value) {
  PHProbe::instance()->setLowpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("Low = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "Low = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState(new Wait(3000, new SeePHCalibration(true)));
}

void PHCalibrationLower::setValue(float value) {
  PHProbe::instance()->setLowpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("Lower = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "Lower = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState(new Wait(3000, new SeePHCalibration(true)));
}
