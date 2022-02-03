/**
 * SeeVersion.cpp
 */

#include "SeeVersion.h"

#include "Devices/LiquidCrystal_TC.h"
#include "TankController.h"

int SeeVersion::writeToBuffer(char* buffer, int size, int line) {
  // Version is null-terminated so no need to use size
  const char* version = TankController::instance()->version();
  if (size > 17) {
    return snprintf_P(buffer, size, (PGM_P)F("\"Version\":\"%s\""), version);
  } else {
    return snprintf_P(buffer, size, (PGM_P)F("%s"), version);
  }
}
void SeeVersion::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char version[17];
  writeToBuffer(version, sizeof(version), 1);
  LiquidCrystal_TC::instance()->writeLine(version, 1);
}
