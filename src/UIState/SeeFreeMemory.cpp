#include "SeeFreeMemory.h"

#include "Devices/LiquidCrystal_TC.h"
#include "TC_util.h"
#include "TankControllerLib.h"

void SeeFreeMemory::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  char buffer[17];
  snprintf(buffer, sizeof(buffer), CSTR("%i bytes"), (int)TankControllerLib::instance()->freeMemory());
  LiquidCrystal_TC::instance()->writeLine(buffer, 1);
}
