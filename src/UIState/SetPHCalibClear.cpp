/**
 * SetCalibrationClear.cpp
 */
#include "SetPHCalibClear.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

void SetPHCalibClear::handleKey(char key) {
  switch (key) {
    case 'A':  // Save (clear calibration)
      PHProbe::instance()->clearCalibration();
      this->setNextState(new Wait(tc, 3000, new SeePHCalibration(tc)));
      break;
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
