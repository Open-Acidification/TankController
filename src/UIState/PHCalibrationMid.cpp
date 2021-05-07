/**
 * PHCalibrationMid.cpp
 */
#include "PHCalibrationMid.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "PHCalibrationLow.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationMid::setValue(double value) {
  PHProbe::instance()->setMidpointCalibration(value);

  char output[17];
  snprintf(output, sizeof(output), "New Mid = %2i.%03i", FLOAT(value, 3));  // "New Mid = 12.345"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  this->setNextState((UIState*)new Wait(tc, 1000, new PHCalibrationLow(tc)));
}
