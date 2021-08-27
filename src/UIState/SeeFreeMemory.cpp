#include "SeeFreeMemory.h"

#include "Devices/LiquidCrystal_TC.h"
#include "TankControllerLib.h"

void SeeFreeMemory::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i bytes"), (int)TankControllerLib::instance()->freeMemory());
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
