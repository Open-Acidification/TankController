/**
 * CalibrationManagement.cpp
 */

#include "CalibrationManagement.h"

void CalibrationManagement::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
