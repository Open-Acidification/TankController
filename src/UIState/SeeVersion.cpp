/**
 * SeeVersion.cpp
 */

#include "SeeVersion.h"

#include "TankController.h"
#include "wrappers/LiquidCrystal_TC.h"

void SeeVersion::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  const char* version = TankController::instance()->version();
  LiquidCrystal_TC::instance()->writeLine(version, 1);
}
