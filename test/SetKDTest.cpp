#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "EEPROM_TC.h"
#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "SetKD.h"
#include "TankController.h"

unittest(test) {
  TankController* tc = TankController::instance();
  SetKD* test = new SetKD();
  tc->setNextState(test, true);
  EEPROM_TC::instance()->setKD(9);
  // getCurrentValue
  assertEqual(9, test->getCurrentValue());
  // setValue
  test->setValue(12345.5);
  assertEqual(12345.5, EEPROM_TC::instance()->getKD());
  // during the delay we showed the new value
  std::vector<String> lines = LiquidCrystal_TC::instance()->getLines();
  assertEqual("New KD=12345.5  ", lines[1]);
  assertEqual("SetKD", tc->stateName());

  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
