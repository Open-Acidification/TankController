/**
 * PHCalibrationLow.cpp
 */
#include "PHCalibrationLow.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationHigh.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationLow::setValue(float value) {
  PHProbe::instance()->setLowpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("New Low = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "New Low = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState((UIState*)new Wait(tc, 3000, new PHCalibrationHigh(tc)));
}
