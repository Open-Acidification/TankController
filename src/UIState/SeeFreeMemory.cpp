#include "SeeFreeMemory.h"

#include "TankController.h"
#include "wrappers/LiquidCrystal_TC.h"

void SeeFreeMemory::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char buffer[17];
  snprintf_P(buffer, sizeof(buffer), (PGM_P)F("%i bytes"), (int)TankController::instance()->freeMemory());
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
