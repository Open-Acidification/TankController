/**
 * PHCalibrationPrompt.cpp
 */
#include "PHCalibrationPrompt.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationMid.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationPrompt::setValue(float value) {
  char buffer[17];
  if(value == 1.0) {
    strscpy_P(buffer, F("1-point pH calib"), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState((UIState*)new Wait(tc, 3000, new PHCalibrationMidOne(tc)));
  } else if (value == 2.0) {
    strscpy_P(buffer, F("2-point pH calib"), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState((UIState*)new Wait(tc, 3000, new PHCalibrationMidTwo(tc)));
  } else if (value == 3.0) {
    strscpy_P(buffer, F("3-point pH calib"), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState((UIState*)new Wait(tc, 3000, new PHCalibrationMidThree(tc)));
  } else {
    strscpy_P(buffer, F("Invalid choice"), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    returnToMainMenu(3000);
  }
}
