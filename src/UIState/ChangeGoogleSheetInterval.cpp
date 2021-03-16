/**
 * ChangeGoogleSheetInterval.cpp
 */

#include "ChangeGoogleSheetInterval.h"

void ChangeGoogleSheetInterval::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
