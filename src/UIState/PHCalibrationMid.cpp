/**
 * PHCalibrationMid.cpp
 */
#include "Devices/PHProbe.h"
#include "Devices/LiquidCrystal_TC.h"
#include "PHCalibrationLow.h"
#include "PHCalibrationMid.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationMid::setValue(double value) {
  PHProbe::instance()->setMidpointCalibration(value);

  char output[17];
  sprintf(output, "New Mid = %2.3f", value);  // "New Mid = 12.345"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  this->setNextState((UIState*)new Wait(tc, 1000, new PHCalibrationLow(tc)));
}
