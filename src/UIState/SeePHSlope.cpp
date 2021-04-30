/**
 * SeePHSlope.cpp
 */

#include "SeePHSlope.h"

#include "LiquidCrystal_TC.h"
#include "PHProbe.h"

void SeePHSlope::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine(PHProbe::instance()->getSlope().c_str(), 1);
}
