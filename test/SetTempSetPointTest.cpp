#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "SetTempSetPoint.h"
#include "TankController.h"

unittest(test) {
  TankController* tc = TankController::instance();
  SetTempSetPoint* test = new SetTempSetPoint(tc);
  assertEqual("MainMenu", tc->stateName());
  tc->setNextState(test, true);
  // EEPROM_TC::instance()->setTemp(4);

  // getCurrentValue
  assertEqual(20, test->getCurrentValue());

  // setValue
  test->setValue(50.25);
  assertEqual(50.25, EEPROM_TC::instance()->getTemp());

  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New Temp=50.25  ", lines[1]);
  assertEqual("SetTempSetPoint", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
