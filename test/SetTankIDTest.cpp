#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "SetTankID.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  SetTankID* test = new SetTankID(tc);
  tc->setNextState(test, true);

  // getCurrentValue
  assertEqual(0, test->getCurrentValue());

  // setValue
  test->setValue(12);
  assertEqual(12, EEPROM_TC::instance()->getTankID());

  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("Tank ID = 12    ", lines[1]);
  assertEqual("SetTankID", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
