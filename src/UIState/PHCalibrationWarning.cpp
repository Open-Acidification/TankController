/**
 * PHCalibrationWarning.cpp
 */

#include "PHCalibrationWarning.h"

#include "Devices/EEPROM_TC.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "TankController.h"
#include "UIState/MainMenu.h"
#include "UIState/SeePHCalibration.h"

PHCalibrationWarning::PHCalibrationWarning() : UIState() {
  startTime = millis();
}

void PHCalibrationWarning::handleKey(char key) {
  switch (key) {
    case 'A':
      EEPROM_TC::instance()->setIgnoreBadPHSlope(true);
      returnToMainMenu();
      break;
    case 'C':
      PHProbe::instance()->clearCalibration();
      this->setNextState(new SeePHCalibration(true));
      break;
    default:
      break;
  }
}

void PHCalibrationWarning::loop() {
  if ((millis() - startTime) % 8000 < 5000) {
    // Blink "BAD CALIBRATION?" in first line and show slope in second line
    if ((millis() - startTime) % 1000 < 700) {
      LiquidCrystal_TC::instance()->writeLine(this->prompt(), 0);
    } else {
      LiquidCrystal_TC::instance()->writeLine(F(""), 0);
    }
    char slopeResponse[20];
    PHProbe::instance()->getSlope(slopeResponse, sizeof(slopeResponse));
    LiquidCrystal_TC::instance()->writeLine(slopeResponse, 1);
  } else {
    // Prompt for response
    LiquidCrystal_TC::instance()->writeLine(F("A: Accept/ignore"), 0);
    LiquidCrystal_TC::instance()->writeLine(F("C: Clear calibra"), 1);
  }
}