/**
 * SetCalibrationClear.cpp
 */
#include "SetCalibrationClear.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "UIState.h"
#include "Wait.h"

void SetCalibrationClear::setValue(double value) {
  if (!(value == 1.0)) {
    LiquidCrystal_TC::instance()->writeLine("Invalid entry!  ", 1);
  } else {
    PHProbe::instance()->clearCalibration();
    LiquidCrystal_TC::instance()->writeLine("Cleared!        ", 1);
  }
}
