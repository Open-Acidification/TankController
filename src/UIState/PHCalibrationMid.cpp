/**
 * PHCalibrationMid.cpp
 */
#include "PHCalibrationMid.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationLow.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationMid::setValue(float value) {
  PHProbe::instance()->setMidpointCalibration(value);
  char buffer[17];
  strscpy_P(buffer, F("New Mid = "), sizeof(buffer));
  floattostrf(value, 5, 3, buffer + strnlen(buffer, sizeof(buffer)), 7);  // "New Mid = 12.345"
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
  this->setNextState((UIState*)new Wait(tc, 3000, new PHCalibrationLow(tc)));
}
