/**
 * ResetLCDScreen.cpp
 */

#include "ResetLCDScreen.h"

void ResetLCDScreen::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
