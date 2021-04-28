/**
 * PHCalibrationHigh.cpp
 */
#include "PHCalibrationHigh.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

void PHCalibrationHigh::setValue(double value) {
  PHProbe::instance()->setHighpointCalibration(value);

  char output[17];
  snprintf(output, sizeof(output), "New High=%2.3f", value);  // "New High=12.345"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
