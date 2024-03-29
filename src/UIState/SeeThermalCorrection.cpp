/**
 * SeeThermalCorrection.cpp
 */

#include "SeeThermalCorrection.h"

#include "ThermalCalibration.h"
#include "model/TC_util.h"
#include "wrappers/LiquidCrystal_TC.h"
#include "wrappers/ThermalProbe_TC.h"

void SeeThermalCorrection::start() {
  // First line: prompt
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  // Second line: the offset (5 digits of precision)
  char buffer[17];
  floattostrf(ThermalProbe_TC::instance()->getCorrection(), 7, 5, buffer, sizeof(buffer));
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
