#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "SetKI.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  SetKI* test = new SetKI(tc);
  tc->setNextState(test, true);
  // setValue
  test->setValue(12345.5);
  assertEqual(12345.5, EEPROM_TC::instance()->getKI());
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New KI=12345.5  ", lines[1]);
  assertEqual("SetKI", tc->stateName());

  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
