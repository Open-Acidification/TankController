/**
 * EnablePID.cpp
 */

#include "EnablePID.h"

void EnablePID::handleKey(char key) {
  switch (key) {
    case 'D':  // Don't save (cancel)
      returnToMainMenu();
      break;
    default:
      break;
  };
}
