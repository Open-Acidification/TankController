/**
 * SeeVersion.cpp
 */

#include "SeeVersion.h"

#include "Devices/LiquidCrystal_TC.h"
#include "TankController.h"

void SeeVersion::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  // const char* version = TankController::instance()->version();
  char output[17];
  snprintf_P(output, sizeof(output), (PGM_P)F("%s"), TankController::instance()->version());
  LiquidCrystal_TC::instance()->writeLine(output, 1);
}
