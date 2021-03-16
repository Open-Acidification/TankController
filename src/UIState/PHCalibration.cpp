/**
 * PHCalibration.cpp
 */

#include "PHCalibration.h"

void PHCalibration::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
