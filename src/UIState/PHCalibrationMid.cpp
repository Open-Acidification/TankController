/**
 * PHCalibrationMid.cpp
 */
#include "PHCalibrationMid.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationHigh.h"
#include "PHCalibrationLow.h"
#include "SeePHCalibration.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationMid::setValue(float value) {
  PHProbe::instance()->setMidpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("Mid = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "Mid = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  // Standard chemistry practice seems to be to set mid -> low -> high. However, mid -> high -> low has the advantage
  // that if the calibrator neglects to replace the probe into the tank after calibration, it will be in a basic
  // solution and the CO2 bubbler will remain off.
  this->setNextState(new Wait(tc, 3000, new PHCalibrationHigh()));
}

void PHCalibrationHigher::setValue(float value) {
  PHProbe::instance()->setMidpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("Higher = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "Higher = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState(new Wait(tc, 3000, new PHCalibrationLower()));
}

void PHCalibrationOnly::setValue(float value) {
  PHProbe::instance()->setMidpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("Buffer = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "Buffer = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState(new Wait(tc, 3000, new SeePHCalibration(true)));
}
