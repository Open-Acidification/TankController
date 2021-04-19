/**
 * SeeVersion.cpp
 */

#include "SeeVersion.h"

#include "../Devices/LiquidCrystal_TC.h"
#include "../TankControllerLib.h"

void SeeVersion::loop() {
  const char* version = TankControllerLib::instance()->version();
  LiquidCrystal_TC::instance()->writeLine(version, 1);
}
