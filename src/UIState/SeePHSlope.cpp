/**
 * SeePHSlope.cpp
 */

#include "SeePHSlope.h"

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHProbe.h"

void SeePHSlope::loop() {
  LiquidCrystal_TC::instance()->writeLine(PHProbe::instance()->getSlope().c_str(), 1);
}
