/**
 * PHCalibrationHigh.cpp
 */
#include "PHCalibrationHigh.h"

#include "wrappers/LiquidCrystal_TC.h"
#include "model/PHProbe.h"
#include "PHCalibrationLow.h"
#include "Wait.h"

void PHCalibrationHigh::setValue(float value) {
  PHProbe::instance()->setHighpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("High = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "High = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState(new Wait(3000, new PHCalibrationLow()));
}
