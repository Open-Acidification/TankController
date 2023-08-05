/**
 * ThermalCalibration.cpp
 */

#include "ThermalCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/ThermalProbe_TC.h"

void ThermalCalibration::setValue(float value) {
  ThermalProbe_TC *pProbe = ThermalProbe_TC::instance();
  float uncorrectedAverage = pProbe->getUncorrectedRunningAverage();
  pProbe->setCorrection(value - uncorrectedAverage);
  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("New correction=%i.%02i"), (int)(value - uncorrectedAverage),
             (int)((value - uncorrectedAverage) * 100 + 0.5) % 100);
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  returnToMainMenu(3000);  // after 3-second delay
}
