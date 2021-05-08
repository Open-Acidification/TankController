/**
 * SetPIDOnOff.cpp
 */

#include "SetPIDOnOff.h"

void SetPIDOnOff::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
