/**
 * PIDTuningMenu.cpp
 */

#include "PIDTuningMenu.h"

void PIDTuningMenu::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
