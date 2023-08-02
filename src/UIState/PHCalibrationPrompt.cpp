/**
 * PHCalibrationPrompt.cpp
 */
#include "PHCalibrationPrompt.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationMid.h"
#include "SeePHCalibration.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationPrompt::setValue(float value) {
  char buffer[17];
  if (value == 1) {
    strscpy_P(buffer, F("1-pt pH calib..."), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState(new Wait(2000, new PHCalibrationOnly()));
  } else if (value == 2) {
    strscpy_P(buffer, F("2-pt pH calib..."), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState(new Wait(2000, new PHCalibrationHigher()));
  } else if (value == 3) {
    strscpy_P(buffer, F("3-pt pH calib..."), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState(new Wait(2000, new PHCalibrationMid()));
  } else {
    strscpy_P(buffer, F("Invalid entry"), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState(new Wait(3000));
  }
}
