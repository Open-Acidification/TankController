#include "SetPHSetPoint.h"

#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "EEPROM_TC.h"
#include "TankControllerLib.h"

unittest(test) {
  TankControllerLib* tc = TankControllerLib::instance();
  EEPROM_TC::instance()->setPH(8.100);
  SetPHSetPoint* test = new SetPHSetPoint(tc);
  assertEqual(8.100, PHControl::instance()->getTargetPh());
  tc->setNextState(test, true);

  // setValue
  test->setValue(7.1234);
  assertEqual(7.1234, PHControl::instance()->getTargetPh());
  assertEqual(7.1234, EEPROM_TC::instance()->getPH());

  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New pH=7.1234   ", lines[1]);
  assertEqual("SetPHSetPoint", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(1000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
