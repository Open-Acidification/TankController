/**
 * SetCalibrationClear.cpp
 */
#include "SetCalibrationClear.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

void SetCalibrationClear::handleKey(char key) {
  switch (key) {
    case 'A':  // Save (clear calibration)
      PHProbe::instance()->clearCalibration();
      LiquidCrystal_TC::instance()->writeLine("Cleared pH Calib", 1);
      returnToMainMenu(3000);
      break;
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
