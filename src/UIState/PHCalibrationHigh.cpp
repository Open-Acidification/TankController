/**
 * PHCalibrationHigh.cpp
 */
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationHigh.h"

void PHCalibrationHigh::setValue(double value) {
  PHProbe::instance()->setHighpointCalibration(value);

  char output[17];
  sprintf(output, "New High=%2.3f", value);  // "New High=12.345"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
