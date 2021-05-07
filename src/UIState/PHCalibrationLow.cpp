/**
 * PHCalibrationLow.cpp
 */
#include "PHCalibrationLow.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"
#include "Devices/Serial_TC.h"
#include "PHCalibrationHigh.h"
#include "UIState.h"
#include "Wait.h"

void PHCalibrationLow::setValue(double value) {
  PHProbe::instance()->setLowpointCalibration(value);

  char output[17];
  snprintf(output, sizeof(output), "New Low = %2i.%03i", FLOAT(value, 3));  // "New Low = 12.345"
  LiquidCrystal_TC::instance()->writeLine(output, 1);
  this->setNextState((UIState*)new Wait(tc, 1000, new PHCalibrationHigh(tc)));
}
