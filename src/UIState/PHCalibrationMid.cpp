/**
 * PHCalibrationMid.cpp
 */
#include "PHCalibrationMid.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "PHCalibrationLow.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationMid::setValue(float value) {
  PHProbe::instance()->setMidpointCalibration(value);

  char output[17];
  snprintf(output, sizeof(output), "New Mid = %2.3f", value);  // "New Mid = 12.345"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  this->setNextState((UIState*)new Wait(tc, 1000, new PHCalibrationLow(tc)));
}
