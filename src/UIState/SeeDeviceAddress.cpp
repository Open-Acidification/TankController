/**
 * SeeDeviceAddress.cpp
 */

#include "SeeDeviceAddress.h"

void SeeDeviceAddress::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
