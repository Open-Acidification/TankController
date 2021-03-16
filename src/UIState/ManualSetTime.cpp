/**
 * ManualSetTime.cpp
 */

#include "ManualSetTime.h"

void ManualSetTime::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
