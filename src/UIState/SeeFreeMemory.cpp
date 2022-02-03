#include "SeeFreeMemory.h"

#include "Devices/LiquidCrystal_TC.h"
#include "TankController.h"
int SeeFreeMemory::writeToBuffer(char* buffer, int size, int line) {
  if (size > 17) {
    return snprintf_P(buffer, size, (PGM_P)F("\"FreeMemory\":\"%i bytes\""),
                      (int)TankController::instance()->freeMemory());
  } else {
    return snprintf_P(buffer, size, (PGM_P)F("%i bytes"), (int)TankController::instance()->freeMemory());
  }
}

void SeeFreeMemory::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char buffer[17];
  writeToBuffer(buffer, sizeof(buffer), 1);
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
