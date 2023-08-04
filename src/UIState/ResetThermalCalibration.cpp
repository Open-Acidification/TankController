/**
 * SetCalibrationClear.cpp
 */
#include "ResetThermalCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/ThermalProbe_TC.h"

void ResetThermalCalibration::handleKey(char key) {
  switch (key) {
    case 'A':  // Save (clear calibration)
      ThermalProbe_TC::instance()->clearCorrection();
      LiquidCrystal_TC::instance()->writeLine(F("Cleared TempCali"), 1);
      returnToMainMenu(3000);
      break;
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
