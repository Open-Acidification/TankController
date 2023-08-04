/**
 * SeeTempCalOffset.cpp
 */

#include "SeeTempCalOffset.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/ThermalProbe_TC.h"
#include "TC_util.h"
#include "TemperatureCalibration.h"

void SeeTempCalOffset::start() {
  // First line: prompt
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  // Second line: the offset (5 digits of precision)
  char buffer[17];
  floattostrf(ThermalProbe_TC::instance()->getCorrection(), 7, 5, buffer, sizeof(buffer));
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
