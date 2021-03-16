/**
 * SeeDeviceUptime.cpp
 */

#include "SeeDeviceUptime.h"

void SeeDeviceUptime::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
