/**
 * SetCalibrationClear.cpp
 */

#include "SetCalibrationClear.h"

void SetCalibrationClear::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
