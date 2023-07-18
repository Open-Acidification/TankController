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
  if (value == 1 || value == 2 || value == 3) {
    snprintf_P(buffer, sizeof(buffer), PSTR("%i-pt pH calib..."), (int)value);
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState(new Wait(tc, 2000, new PHCalibrationMid(tc, value)));
  } else {
    strscpy_P(buffer, F("Invalid entry"), sizeof(buffer));
    LiquidCrystal_TC::instance()->writeLine(buffer, 1);
    this->setNextState(new Wait(tc, 3000, new SeePHCalibration(tc)));
  }
}
