/**
 * TemperatureCalibration.cpp
 */

#include "TemperatureCalibration.h"

void TemperatureCalibration::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
