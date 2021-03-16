/**
 * ChangeTankID.cpp
 */

#include "ChangeTankID.h"

void ChangeTankID::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
