/**
 * SeeLogFile.cpp
 */

#include "SeeLogFile.h"

#include "../TC_util.h"
#include "Devices/LiquidCrystal_TC.h"
#include "Devices/SD_TC.h"

void SeeLogFile::start() {
  LiquidCrystal_TC::instance()->writeLine(prompt(), 0);
  LiquidCrystal_TC::instance()->writeLine(SD_TC::instance()->todaysDataFileName().c_str(), 1);
}
