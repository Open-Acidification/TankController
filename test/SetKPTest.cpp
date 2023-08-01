#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "SetKP.h"
#include "TankController.h"

unittest(test) {
  TankController* tc = TankController::instance();
  SetKP* test = new SetKP();
  tc->setNextState(test, true);
  EEPROM_TC::instance()->setKP(7);

  // getCurrentValue
  assertEqual(7, test->getCurrentValue());

  // setValue
  test->setValue(123456.5);
  assertEqual(123456.5, EEPROM_TC::instance()->getKP());

  // show the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New KP=123456.5 ", lines[1]);
  assertEqual("SetKP", tc->stateName());

  tc->loop(false);  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop(false);  // queue MainMenu to be next
  tc->loop(false);  // transition to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
