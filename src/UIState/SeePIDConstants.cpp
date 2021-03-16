/**
 * SeePIDConstants.cpp
 */

#include "SeePIDConstants.h"

void SeePIDConstants::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
