/**
 * SetCalibrationClear.cpp
 */
#include "SetTempCalibClear.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/TempProbe_TC.h"

void SetTempCalibClear::handleKey(char key) {
  switch (key) {
    case 'A':  // Save (clear calibration)
      TempProbe_TC::instance()->clearCorrection();
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
