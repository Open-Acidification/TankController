#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "EEPROM_TC.h"
#include "SetPHSetPoint.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->resetEEPROM();
}

unittest(test) {
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  EEPROM_TC::instance()->setPH(8.125);
  EEPROM_TC::instance()->setRampTimeEnd(0);
  EEPROM_TC::instance()->setRampTimeStart(0);
  TankController* tc = TankController::instance();
  assertEqual(8.125, EEPROM_TC::instance()->getPH());
  assertEqual(0, 8.125 - EEPROM_TC::instance()->getPH());
  assertEqual(8.125, PHControl::instance()->getTargetPh());
  assertEqual(0, EEPROM_TC::instance()->getRampTimeEnd());
  assertEqual(0, PHControl::instance()->getRampTimeEnd());
  assertEqual(0, EEPROM_TC::instance()->getRampTimeStart());
  assertEqual(0, PHControl::instance()->getRampTimeStart());
  SetPHSetPoint* test = new SetPHSetPoint(tc);
  tc->setNextState(test, true);

  // get currently displayed lines
  std::vector<String> lines = lcd->getLines();
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual("Set pH Set Point", lines.at(0));
  assertEqual("  8.125->     0 ", lines.at(1));
  assertEqual(8.125, test->getCurrentValue());

  // setValue
  test->setValue(7.125);

  lines = lcd->getLines();
  assertEqual("Set ramp hours: ", lines.at(0));
  assertEqual("    0.0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(4.125);

  // during the delay we showed the new value
  lines = lcd->getLines();
  assertEqual(7.125, PHControl::instance()->getTargetPh());
  assertEqual(7.125, EEPROM_TC::instance()->getPH());

  assertEqual("New pH=7.125    ", lines[0]);
  assertEqual("New ramp=4.125  ", lines[1]);
  assertEqual("SetPHSetPoint", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
