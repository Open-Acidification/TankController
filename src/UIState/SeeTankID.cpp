/**
 * SeeTankID.cpp
 */

#include "SeeTankID.h"

void SeeTankID::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
