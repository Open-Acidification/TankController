/**
 * SetCalibrationClear.cpp
 */
#include "ResetPHCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "SeePHCalibration.h"
#include "Wait.h"

void ResetPHCalibration::handleKey(char key) {
  switch (key) {
    case 'A':  // Save (clear calibration)
      PHProbe::instance()->clearCalibration();
      this->setNextState(new SeePHCalibration(false));
      break;
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
