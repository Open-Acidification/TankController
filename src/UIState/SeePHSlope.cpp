/**
 * SeePHSlope.cpp
 */

#include "SeePHSlope.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

void SeePHSlope::loop() {
  LiquidCrystal_TC::instance()->writeLine(PHProbe::instance()->getSlope().c_str(), 1);
}

void SeePHSlope::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine("requesting slope", 1);
  PHProbe::instance()->sendSlopeRequest();
}