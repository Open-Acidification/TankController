/**
 * SeePHSlope.cpp
 */

#include "SeePHSlope.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

void SeePHSlope::loop() {
  char buffer[20];
  PHProbe::instance()->getSlope(buffer, sizeof(buffer));
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}

void SeePHSlope::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine("requesting slope", 1);
  PHProbe::instance()->sendSlopeRequest();
}
