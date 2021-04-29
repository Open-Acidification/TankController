/**
 * SetCalibrationClear.cpp
 */

#include "SetCalibrationClear.h"

void SetChillOrHeat::setValue(double value) {
  if (!(value == 1.0) {
    LiquidCrystal_TC::instance()->writeLine("Invalid entry!  ", 1);
  } else {
    PHProbe::instance()->clearCalibration();
    LiquidCrystal_TC::instance()->writeLine("Cleared!        ", 1);
  }
  returnToMainMenu(1000);  // after 1-second delay
}
