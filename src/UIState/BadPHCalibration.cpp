/**
 * BadPHCalibration.cpp
 */

#include "BadPHCalibration.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "UIState/MainMenu.h"
#include "UIState/SeePHCalibration.h"

BadPHCalibration::BadPHCalibration(TankController* tc) : UIState(tc) {
  startTime = millis();
}

void BadPHCalibration::handleKey(char key) {
  switch (key) {
    case 'A':
      // warnForBadCalibration = false;
      returnToMainMenu();
      break;
    case 'C':
      // warnForBadCalibration = true;
      PHProbe::instance()->clearCalibration();
      this->setNextState(new SeePHCalibration(tc, true));
      break;
    default:
      break;
  }
}

void BadPHCalibration::loop() {
  if ((millis() - startTime) % 8000 < 5000) {
    // Blink "BAD CALIBRATION?" in first line and show slope in second line
    if ((millis() - startTime) % 1000 < 750) {
      LiquidCrystal_TC::instance()->writeLine(this->prompt(), 0);
    } else {
      LiquidCrystal_TC::instance()->writeLine(F(""), 0);
    }
    char slopeResponse[20];
    PHProbe::instance()->getSlope(slopeResponse, sizeof(slopeResponse));
    LiquidCrystal_TC::instance()->writeLine(slopeResponse, 1);
  } else {
    // Prompt for response
    LiquidCrystal_TC::instance()->writeLine(F("A: Accept anyway"), 0);
    LiquidCrystal_TC::instance()->writeLine(F("C: Clear/reset"), 1);
  }
}