/**
 * PHCalibrationLow.cpp
 */
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationLow.h"

void PHCalibrationLow::setValue(double value) {
  PHProbe::instance()->setLowpointCalibration(value);

  char output[17];
  sprintf(output, "New Low = %2.3f", value);  // "New Low = 12.345"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
