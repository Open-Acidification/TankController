/**
 * SetChillOrHeat.cpp
 */

#include "SetChillOrHeat.h"

void SetChillOrHeat::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
