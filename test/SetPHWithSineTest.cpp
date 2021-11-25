#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Devices/LiquidCrystal_TC.h"
#include "Devices/PHControl.h"
#include "EEPROM_TC.h"
#include "SetPHWithSine.h"
#include "TC_util.h"
#include "TankController.h"

unittest_setup() {
  GODMODE()->resetEEPROM();
}

unittest(test) {
  int PH_SIN = 2;
  LiquidCrystal_TC* lcd = LiquidCrystal_TC::instance();
  TankController* tc = TankController::instance();
  SetPHWithSine* test = new SetPHWithSine(tc);
  tc->setNextState(test, true);
  EEPROM_TC::instance()->setPH(8.125);
  assertEqual(EEPROM_TC::instance()->getPHSetType(), 0);

  // get currently displayed lines
  std::vector<String> lines = lcd->getLines();
  assertEqual(16, lines.at(0).length());
  assertEqual(16, lines.at(1).length());
  assertEqual("Set pH Set Point", lines.at(0));
  assertEqual("  8.125->     0 ", lines.at(1));
  assertEqual(8.125, test->getCurrentValue());

  // setValues
  test->setValue(7.125);

  lines = lcd->getLines();
  assertEqual("Set Amplitude:  ", lines.at(0));
  assertEqual("    0.0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(2.125);

  lines = lcd->getLines();
  assertEqual("Set Period: ", lines.at(0));
  assertEqual("    0.0->     0 ", lines.at(1));
  assertEqual(0, test->getCurrentValue());
  test->setValue(1.5);

  // during the delay we showed the new value
  lines = lcd->getLines();
  assertEqual(7.125, PHControl::instance()->getTargetPh());
  assertEqual(7.125, EEPROM_TC::instance()->getPH());
  assertEqual(PH_SIN, EEPROM_TC::instance()->getPHSetType());

  assertEqual("New pH=7.125    ", lines[0]);
  assertEqual("A=2.125 P=1.5   ", lines[1]);
  assertEqual("SetPHWithSine", tc->stateName());
  tc->loop();  // transition to Wait
  assertEqual("Wait", tc->stateName());
  delay(3000);
  tc->loop();  // queue MainMenu to be next
  tc->loop();  // transition to MainMenu
  // now we should be back to the main menu
  assertEqual("MainMenu", tc->stateName());
}

unittest_main()
