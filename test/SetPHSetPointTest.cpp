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

unittest(test_target_of_7_125_with_ramp_of_4_125) {
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  EEPROM_TC::instance()->setPh(8.125);
  EEPROM_TC::instance()->setPhRampTimeEnd(0);
  EEPROM_TC::instance()->setPhRampTimeStart(0);
  TankController* tc = TankController::instance();  // instantiate after setting eeprom stuff
  assertEqual(PHControl::instance()->phSetTypeTypes::FLAT_TYPE, PHControl::instance()->getPhSetType());
  assertEqual(8.125, EEPROM_TC::instance()->getPh());
  assertEqual(0, 8.125 - EEPROM_TC::instance()->getPh());
  assertEqual(8.125, PHControl::instance()->getTargetPh());
  assertEqual(0, EEPROM_TC::instance()->getPhRampTimeEnd());
  assertEqual(0, EEPROM_TC::instance()->getPhRampTimeStart());
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
  assertEqual(7.125, EEPROM_TC::instance()->getPh());
  assertEqual(PHControl::instance()->phSetTypeTypes::RAMP_TYPE, EEPROM_TC::instance()->getPhSetType());

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

unittest(test_target_of_14_with_ramp_of_0) {
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  SetPHSetPoint* test = new SetPHSetPoint(tc);
  tc->setNextState(test, true);

  // setValue
  test->setValue(14);
  test->setValue(0);

  // during the delay we showed the new value
  assertEqual(14, PHControl::instance()->getTargetPh());
  assertEqual(14, EEPROM_TC::instance()->getPh());
  assertEqual(PHControl::instance()->phSetTypeTypes::FLAT_TYPE, EEPROM_TC::instance()->getPhSetType());
  std::vector<String> lines = lcd->getLines();
  assertEqual("New pH=14.000   ", lines[0]);
  assertEqual("New ramp=0.000  ", lines[1]);

  // complete cycle back to main menu
  assertEqual("SetPHSetPoint", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
  // note that the following shows only two digits after the decimal
  assertEqual("pH=0.000   14.00", lcd->getLines().at(0));
}

unittest_main()
