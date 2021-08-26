/**
 * TemperatureCalibration.cpp
 */

#include "TemperatureCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TempProbe_TC.h"
#include "TC_util.h"

void TemperatureCalibration::setValue(float value) {
  TempProbe_TC *pProbe = TempProbe_TC::instance();
  float uncorrectedAverage = pProbe->getUncorrectedRunningAverage();
  pProbe->setCorrection(value - uncorrectedAverage);
  char output[17];
  snprintf(output, sizeof(output), CSTR("New correction=%.2f"), value - uncorrectedAverage);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);  // after 3-second delay
}
